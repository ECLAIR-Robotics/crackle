#include <Arduino.h>
#include "linear_algebra.hpp"

// 4 microphones in square pattern
// (4 choose 2) = 6 so 6 cones

const int MIC_ZERO = A0;
const int MIC_ONE = A1;
const int MIC_TWO = A2;
const int MIC_THREE = A3;

const int threshold = 100;
const long voltage = 330;

Vector mic_locations[4] = { // in METERS
  Vector(0.06, 0.06, 0), // 0
  Vector(-0.06, 0.06, 0), // 1
  Vector(-0.06, -0.06, 0), // 2
  Vector(0.06, -0.06, 0) // 3
};

int mic_pairings[6][2] = {
  {0, 1},
  {0, 2},
  {0, 3},
  {1, 2},
  {1, 3},
  {2, 3}
};

struct Cone {
  Vector normal;
  float slope;
  Cone(Vector n, float s) : normal(n), slope(s) {};
};

// float d = 125/(2*pow(10,3)); // half mic seperation in m
float c = 343; // speed of sound in m/s

const int samples = 450;
const int cross_correlation_range = 25;
unsigned long startTimestamp;
int reads[4][samples];
unsigned int timestamps[4][samples];
int index = 0;
bool reading = true;
// bool serialAudioOutput = false;
bool serialAudioOutput = true;

void setup() {
  Serial.begin(19200);
  ADCSRA = (ADCSRA & 0xF8) | 0x02; // Set Analogue Read Rate (change to 0x02 if want faster)
  ADCSRA &= ~(1 << ADATE); // Disable auto triggering
  pinMode(MIC_ZERO, INPUT);
  pinMode(MIC_ONE, INPUT);
  pinMode(MIC_TWO, INPUT);
  pinMode(MIC_THREE, INPUT);
}

int absolute(int x) {
  if (x < 0) {
    return -x;
  }
  return x;
}

void loop() {
  int zeroRead = analogRead(MIC_ZERO);
  unsigned long zeroTimestamp = micros();
  int oneRead = analogRead(MIC_ONE);
  unsigned long oneTimestamp = micros();
  int twoRead = analogRead(MIC_TWO);
  unsigned long twoTimestamp = micros();
  int threeRead = analogRead(MIC_THREE);
  unsigned long threeTimestamp = micros();
  if (reading) {
    reads[0][index] = zeroRead;
    timestamps[0][index] = zeroTimestamp - startTimestamp;
    reads[1][index] = oneRead;
    timestamps[1][index] = oneTimestamp - startTimestamp;
    reads[2][index] = twoRead;
    timestamps[2][index] = twoTimestamp - startTimestamp;
    reads[3][index] = threeRead;
    timestamps[3][index] = threeTimestamp - startTimestamp;
    index++;
    if (index >= samples) {
      if (serialAudioOutput) {
        Serial.println("0 read, 0 timestamp, 1 read, 1 timestamp, 2 read, 2 timestamp, 3 read, 3 timestamp");
        for (int i = 0; i < samples; i++) {
          char csvLine[100] = "";
          char zeroReadstr[25];
          char zeroTimestampstr[25];
          char oneReadstr[25];
          char oneTimestampstr[25];
          char twoReadstr[25];
          char twoTimestampstr[25];
          char threeReadstr[25];
          char threeTimestampstr[25];
          sprintf(zeroReadstr, "%d", reads[0][i]);
          sprintf(zeroTimestampstr, "%d", timestamps[0][i]);
          sprintf(oneReadstr, "%d", reads[1][i]);
          sprintf(oneTimestampstr, "%d", timestamps[1][i]);
          sprintf(twoReadstr, "%d", reads[2][i]);
          sprintf(twoTimestampstr, "%d", timestamps[2][i]);
          sprintf(threeReadstr, "%d", reads[3][i]);
          sprintf(threeTimestampstr, "%d", timestamps[3][i]);
          strcat(csvLine, zeroReadstr);
          strcat(csvLine, ",");
          strcat(csvLine, zeroTimestampstr);
          strcat(csvLine, ",");
          strcat(csvLine, oneReadstr);
          strcat(csvLine, ",");
          strcat(csvLine, oneTimestampstr);
          strcat(csvLine, ",");
          strcat(csvLine, twoReadstr);
          strcat(csvLine, ",");
          strcat(csvLine, twoTimestampstr);
          strcat(csvLine, ",");
          strcat(csvLine, threeReadstr);
          strcat(csvLine, ",");
          strcat(csvLine, threeTimestampstr);
          Serial.println(csvLine);
        }
      }
      reading = false;
      // Serial.println("1");
      // double x = cross_correlation_time_offset(leftReads, leftTimestamps, rightReads, rightTimestamps);
      // char xstr[25];
      // dtostrf(x, 10, 15, xstr);
      // Serial.println(xstr);
      // Serial.println(get_angle_from_time_offset(cross_correlation_time_offset(leftReads, leftTimestamps, rightReads, rightTimestamps)));
    }
  }
  // } else if (!reading && (absolute(leftRead-voltage) > threshold || absolute(rightRead-voltage) > threshold)) {
  //   index = 0;
  //   reading = true;
  //   startTimestamp = leftTimestamp;
  // }
}

int sign(double x) {
  if (x < 0) {
    return -1;
  } else if (x > 0) {
    return 1;
  }
  return 0;
}

double absolute(double x) {
  if (x < 0) {
    return x * -1;
  }
  return x;
}

double get_slope_from_time_offset(double t, double d) {
  if (d < c*t) {
    return 0;
  }
  return sqrt(pow(d,2)-pow(c*t,2))/(c*t);
}

Cone get_cone(int pairing[2]) {
  double time_offset = cross_correlation_time_offset(reads[pairing[0]], timestamps[pairing[0]], reads[pairing[1]], timestamps[pairing[1]]);
  double distance_between_mics = magnitude(mic_locations[pairing[1]]-mic_locations[pairing[0]]);
  return Cone(
    normalize(mic_locations[pairing[1]]-mic_locations[pairing[0]]) * sign(time_offset),
    get_slope_from_time_offset(absolute(time_offset), distance_between_mics)
  );
}

Vector get_cone_intersection(Cone c1, Cone c2) {

  float alpha = atan(c1.slope);
  float beta = atan(c2.slope);

  float theta = acos(dot(c1.normal, c2.normal));

  Vector n2_cross_n1 = cross(c2.normal, c1.normal);

  if (alpha + beta < theta) {
    float angle_to_cones_midpoint = beta + (theta - alpha - beta)/2;

    return matrix_multiplication(rotaion_matrix_around_axis(n2_cross_n1, angle_to_cones_midpoint), c2.normal);
  }

  float z = cos(alpha);
  Vector w = Vector(0, sin(theta-beta), cos(theta-beta));

  Vector n = Vector(0, sin(theta), cos(theta)); // n2 rotated away from n1 on z-axis

  float angle_of_intersection = cos((z-w.y*n.y*n.z-pow(n.z,2)*w.z)/(w.z-w.y*n.y*n.z-pow(n.z,2)*w.z));

  Vector v = matrix_multiplication(rotaion_matrix_around_axis(n2_cross_n1, beta), c2.normal);

  Vector intersection_direction1 = matrix_multiplication(rotaion_matrix_around_axis(c2.normal, angle_of_intersection), v);
  Vector intersection_direction2 = matrix_multiplication(rotaion_matrix_around_axis(c2.normal, -angle_of_intersection), v);

  if (intersection_direction1.z > 0) { // we assume our sound is in front of mic array. This assumption will not hold in the future when we fix the 3D issues
      return intersection_direction1;
  } else {
    return intersection_direction2;
  }
}

Vector get_sound_direction(int reads[4][samples], unsigned int timestamps[4][samples]) {
  Cone cones[6];
  for (int i=0; i < 6; i++) {
    cones[i] = get_cone(mic_pairings[i]);
  }

  Vector cone_intersection_direction_sum = Vector(0,0,0);

  for (int i=0; i < 5; i++) {
    for (int j=0; j < 6; j++) {
      cone_intersection_direction_sum = cone_intersection_direction_sum + get_cone_intersection(cones[i], cones[j]);
    }
  }

  return normalize(cone_intersection_direction_sum);
}

// left is negative, right is positive
double cross_correlation_time_offset(int aReads[samples], unsigned int aTimestamps[samples], int bReads[samples], unsigned int bTimestamps[samples]) {
  double time_step = pow(10,-6)*(((long) aTimestamps[samples-1]-aTimestamps[0]) + ((long) bTimestamps[samples-1]-bTimestamps[0]))/(2*(samples-1));

  long cross_correlation[2*cross_correlation_range];

  for (int offset = -cross_correlation_range; offset <= cross_correlation_range; offset++) {
    long correlation = 0;
    for (int i = 0; i < samples; i++) {
      if ((0 <= i+offset) && (i+offset < samples)) {
        correlation += (((long) bReads[i]) - voltage)*(((long) aReads[i+offset]) - voltage);
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
  
  double interpolated_max_cross_correlation_time = time_step*(interpolated_max_cross_correlation-cross_correlation_range-0.5);

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

// // left is negative, right is positive
// // 0 is middle
// // t is time, d is distance between mics
// double get_angle_from_time_offset(double t, double d) {
//   // char tstr[25];
//   // dtostrf(t, 10, 15, tstr);
//   // Serial.println(tstr);

//   // t is time difference in seconds
//   // Serial.println("we are here now");
//   int left_right_multiplier;
//   if (t < 0) {
//     left_right_multiplier = -1;
//   } else {
//     left_right_multiplier = 1;
//   }

//   if (t == 0) {
//     return 0;
//   }
//   if (4*pow(d,2) < pow(c*t,2)) {
//     return 90*left_right_multiplier;
//   }

//   return (90-(180/PI)*atan(sqrt((4*pow(d,2)-pow(c*t,2))/pow(c*t,2))))*left_right_multiplier;
// }