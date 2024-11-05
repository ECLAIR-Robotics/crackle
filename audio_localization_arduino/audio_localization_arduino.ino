#include <Arduino.h>

const int MIC_LEFT = A0;
const int MIC_RIGHT = A15;

const int threshold = 100;
const long voltage = 330;

float d = 125/(2*pow(10,3)); // half mic seperation in m
float c = 343; // speed of sound in m/s
float d2 = d*d;

const int samples = 900;
unsigned long startTimestamp;
int leftReads[samples];
unsigned int leftTimestamps[samples];
int rightReads[samples];
unsigned int rightTimestamps[samples];
int index = 0;
bool reading = true;
bool serialAudioOutput = false;
// bool serialAudioOutput = true;

void setup() {
  Serial.begin(19200);
  ADCSRA = (ADCSRA & 0xF8) | 0x02; // Set Analogue Read Rate
  ADCSRA &= ~(1 << ADATE); // Disable auto triggering
  pinMode(MIC_RIGHT, INPUT);
  pinMode(MIC_LEFT, INPUT);
}

int absolute(int x) {
  if (x < 0) {
    return -x;
  }
  return x;
}

void loop() {
  int leftRead = analogRead(MIC_LEFT);
  unsigned long leftTimestamp = micros();
  int rightRead = analogRead(MIC_RIGHT);
  unsigned long rightTimestamp = micros();
  if (reading) {
    leftReads[index] = leftRead;
    leftTimestamps[index] = leftTimestamp;
    rightReads[index] = rightRead;
    rightTimestamps[index] = rightTimestamp;
    index++;
    if (index >= samples) {
      if (serialAudioOutput) {
        for (int i = 0; i < samples; i++) {
          char csvLine[100] = "";
          char leftReadstr[25];
          char leftTimestampstr[25];
          char rightReadstr[25];
          char rightTimestampstr[25];
          sprintf(leftReadstr, "%d", leftReads[i]);
          sprintf(leftTimestampstr, "%d", leftTimestamps[i]);
          sprintf(rightReadstr, "%d", rightReads[i]);
          sprintf(rightTimestampstr, "%d", rightTimestamps[i]);
          strcat(csvLine, leftReadstr);
          strcat(csvLine, ",");
          strcat(csvLine, leftTimestampstr);
          strcat(csvLine, ",");
          strcat(csvLine, rightReadstr);
          strcat(csvLine, ",");
          strcat(csvLine, rightTimestampstr);
          Serial.println(csvLine);
        }
      }
      reading = false;
      Serial.println(get_angle_from_time_offset(cross_correlation_time_offset(leftReads, leftTimestamps, rightReads, rightTimestamps)));
    }
  } else if (!reading && (absolute(leftRead-voltage) > threshold || absolute(rightRead-voltage) > threshold)) {
    index = 0;
    reading = true;
    startTimestamp = leftTimestamp;
  }
}

// left is negative, right is positive
double cross_correlation_time_offset(int leftReads[samples], unsigned int leftTimestamps[samples], int rightReads[samples], unsigned int rightTimestamps[samples]) {
  double time_step = pow(10,-6)*(leftTimestamps[samples-2]-leftTimestamps[samples-1]+rightTimestamps[samples-2]-rightTimestamps[samples-1])/(2*samples);
  long cross_correlation[2*samples];
  for (int offset = -samples; offset <= samples; offset++) {
    long correlation = 0;
    for (int i = 0; i < samples; i++) {
      if ((0 <= i+offset) && (i+offset < samples)) {
        correlation += (((long) rightReads[i]) - voltage)*(((long) leftReads[i+offset]) - voltage);
      }
    }
    cross_correlation[offset+samples] = correlation;
  }

  // for (int i = 0; i < 2*samples; i++) {
  //   Serial.println(cross_correlation[i]);
  // }

  int max_cross_correlation_index = 0;
  int max_cross_correlation = cross_correlation[max_cross_correlation_index];
  for (int i = 0; i <= sizeof(cross_correlation); i++) {
    
    if (cross_correlation[i] > max_cross_correlation) {
      max_cross_correlation_index = i;
      max_cross_correlation = cross_correlation[i];
    }
  }

  // Serial.println(max_cross_correlation_index);

  // https://pages.hmc.edu/ruye/MachineLearning/lectures/ch3/node4.html
  // Parabolic interpolation

  int a = max_cross_correlation_index-1;
  int b = max_cross_correlation_index;
  int c = max_cross_correlation_index+1;
  int fa = cross_correlation[a];
  int fb = cross_correlation[b];
  int fc = cross_correlation[c];
  double interpolated_max_cross_correlation = b+0.5*((fa-fb)*pow(c-b,2)-(fc-fb)*pow(b-a,2))/((fa-fb)*(c-b)+(fc-fb)*(b-a));

  // char tstr[25];
  // dtostrf(time_step*(interpolated_max_cross_correlation-samples-0.5), 10, 15, tstr);
  // Serial.println(tstr);
    
  return time_step*(interpolated_max_cross_correlation-samples-0.5);
}

// left is negative, right is positive
// 0 is middle
double get_angle_from_time_offset(double t) {
    // t is time difference in seconds
    // Serial.println("we are here now");
    int left_right_multiplier;
    if (t < 0) {
      left_right_multiplier = -1;
    } else {
      left_right_multiplier = 1;
    }

    if (t == 0) {
      return 0;
    }
    float m = c*c*t*t;
    if (4*d2 < m) {
      return 90*left_right_multiplier;
    }

    return (90-(180/PI)*atan(sqrt((4*d2-m)/m)))*left_right_multiplier;
}