#include <Arduino.h>

const int MIC_LEFT = A0;
const int MIC_RIGHT = A15;

const int threshold = 100;
const long voltage = 330;

float d = 125/(2*pow(10,3)); // half mic seperation in m
float c = 343; // speed of sound in m/s

const int samples = 900;
const int cross_correlation_range = 25;
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
  ADCSRA = (ADCSRA & 0xF8) | 0x04; // Set Analogue Read Rate (change to 0x02 if want faster)
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
    leftTimestamps[index] = leftTimestamp - startTimestamp;
    rightReads[index] = rightRead;
    rightTimestamps[index] = rightTimestamp - startTimestamp;
    index++;
    if (index >= samples) {
      if (serialAudioOutput) {
        Serial.println("left read, left timestamp, right read, right timestamp");
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
      // Serial.println("1");
      // double x = cross_correlation_time_offset(leftReads, leftTimestamps, rightReads, rightTimestamps);
      // char xstr[25];
      // dtostrf(x, 10, 15, xstr);
      // Serial.println(xstr);
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
  double time_step = pow(10,-6)*(((long) leftTimestamps[samples-1]-leftTimestamps[0]) + ((long) rightTimestamps[samples-1]-rightTimestamps[0]))/(2*(samples-1));
  // Serial.println("here");
  // Serial.println(leftTimestamps[samples-1]);
  // Serial.println(leftTimestamps[0]);
  // Serial.println(rightTimestamps[samples-1]);
  // Serial.println(rightTimestamps[0]);

  // char tstr[25];
  // dtostrf(time_step, 10, 15, tstr);
  // Serial.println(tstr);

  long cross_correlation[2*cross_correlation_range];

  for (int offset = -cross_correlation_range; offset <= cross_correlation_range; offset++) {
    long correlation = 0;
    for (int i = 0; i < samples; i++) {
      if ((0 <= i+offset) && (i+offset < samples)) {
        correlation += (((long) rightReads[i]) - voltage)*(((long) leftReads[i+offset]) - voltage);
      }
    }
    cross_correlation[offset+cross_correlation_range] = correlation;
  }


  int max_cross_correlation_index = 0;
  long max_cross_correlation = cross_correlation[max_cross_correlation_index];
  for (int i = 0; i <= 2*cross_correlation_range; i++) {
    if (cross_correlation[i] > max_cross_correlation) {
      max_cross_correlation_index = i;
      max_cross_correlation = cross_correlation[i];
    }
  }

  // https://pages.hmc.edu/ruye/MachineLearning/lectures/ch3/node4.html
  // Parabolic interpolation

  int a = max_cross_correlation_index-1;
  int b = max_cross_correlation_index;
  int c = max_cross_correlation_index+1;
  long fa = cross_correlation[a];
  long fb = cross_correlation[b];
  long fc = cross_correlation[c];
  double interpolated_max_cross_correlation = b+0.5*((fa-fb)*pow(c-b,2)-(fc-fb)*pow(b-a,2))/((fa-fb)*(c-b)+(fc-fb)*(b-a));

  // har tstr[25];
  // dtostrf(time_step, 10, 15, tstr);
  // Serial.println(tstr);

  double interpolated_max_cross_correlation_time = time_step*(interpolated_max_cross_correlation-cross_correlation_range-0.5);

  // Serial.println("1");

  // char tstr[25];
  // dtostrf(interpolated_max_cross_correlation_time, 10, 15, tstr);
  // Serial.println(tstr);

  // char timestr[25];
  // dtostrf(time_step, 10, 15, timestr);
  // Serial.println(timestr);

  // Serial.println(interpolated_max_cross_correlation);
  // Serial.println(cross_correlation_range);

  return interpolated_max_cross_correlation_time;
}

// left is negative, right is positive
// 0 is middle
double get_angle_from_time_offset(double t) {
  // char tstr[25];
  // dtostrf(t, 10, 15, tstr);
  // Serial.println(tstr);

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
  if (4*pow(d,2) < pow(c*t,2)) {
    return 90*left_right_multiplier;
  }

  return (90-(180/PI)*atan(sqrt((4*pow(d,2)-pow(c*t,2))/pow(c*t,2))))*left_right_multiplier;
}