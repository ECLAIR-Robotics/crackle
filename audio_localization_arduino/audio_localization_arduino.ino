#include <Arduino.h>
#include "linear_algebra.hpp"

// 4 microphones in square pattern
// (4 choose 2) = 6 so 6 cones

const int MIC_ZERO = A0;
const int MIC_ONE = A1;
const int MIC_TWO = A2;
const int MIC_THREE = A3;

const int threshold = 100;
const int voltage = 330;

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

// sound coming from [-16,10,7]
// double mock_time_differences[6] = {
//   0.00027856163755510255,
//   0.00010430549200940442,
//   -0.00017343200226522387,
//   -0.00017425614554569813,
//   -0.0004519936398203264,
//   -0.0002777374942746283
// };

struct Cone {
  Vector normal;
  double slope;
  Cone() : normal(Vector()), slope(0) {};
  Cone(Vector n, double s) : normal(n), slope(s) {};
};

Cone cones[6];

// float d = 125/(2*pow(10,3)); // half mic seperation in m
double c = 343; // speed of sound in m/s

const int samples = 300;
const int cross_correlation_range = 25;
unsigned long startTimestamp;
int reads[4][samples];
unsigned int timestamps[4][samples];
int index = 0;
bool reading = true;
bool serialAudioOutput = false;
// bool serialAudioOutput = true;

void setup() {
  Serial.begin(19200);
  ADCSRA = (ADCSRA & 0xF8) | 0x02; // Set Analogue Read Rate (change to 0x02 if want faster)
  ADCSRA &= ~(1 << ADATE); // Disable auto triggering
  pinMode(MIC_ZERO, INPUT);
  pinMode(MIC_ONE, INPUT);
  pinMode(MIC_TWO, INPUT);
  pinMode(MIC_THREE, INPUT);
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
      Vector sound_direction = get_sound_direction(reads, timestamps);
      String sound_direction_string = "";
      sound_direction_string += "Vector(";
      sound_direction_string += String(sound_direction.x, 4);
      sound_direction_string += ",";
      sound_direction_string += String(sound_direction.y, 4);
      sound_direction_string += ",";
      sound_direction_string += String(sound_direction.z, 4);
      sound_direction_string += ")";
      Serial.println(sound_direction_string);
      reading = false;
      // Serial.println("1");
      // double x = cross_correlation_time_offset(leftReads, leftTimestamps, rightReads, rightTimestamps);
      // char xstr[25];
      // dtostrf(x, 10, 15, xstr);
      // Serial.println(xstr);
      // Serial.println(get_angle_from_time_offset(cross_correlation_time_offset(leftReads, leftTimestamps, rightReads, rightTimestamps)));
    }
  // }s
  } else if (!reading && (((absolute<int>(zeroRead - voltage) + absolute<int>(oneRead - voltage) + absolute<int>(twoRead - voltage) + absolute<int>(threeRead - voltage))/4) > threshold)) {
    index = 0;
    reading = true;
    startTimestamp = zeroTimestamp;
  }
}

int sign(double x) {
  if (x < 0) {
    return -1;
  } else if (x > 0) {
    return 1;
  }
  return 0;
}

template <typename T>
T absolute(T x) {
  if (x < 0) {
    return x * -1;
  }
  return x;
}

double get_slope_from_time_offset(double t, double d) {
  if (d < c*t) {
    return 0;
  }
  // Serial.println("get_slope_from_time_offset");
  // Serial.println(t);
  // Serial.println(sqrt(pow(d,2)-pow(c*t,2))/(c*t));
  return sqrt(pow(d,2)-pow(c*t,2))/(c*t);
}

Vector get_cone_intersection(Cone c1, Cone c2) {

  // Serial.println("c1.normal");
  // Serial.println(c1.normal.x);
  // Serial.println(c1.normal.y);
  // Serial.println(c1.normal.z);
  // Serial.println("c1.slope");
  // Serial.println(c1.slope);

  // Serial.println("c2.normal");
  // Serial.println(c2.normal.x);
  // Serial.println(c2.normal.y);
  // Serial.println(c2.normal.z);
  // Serial.println("c2.slope");
  // Serial.println(c2.slope);

  double alpha = atan(c1.slope);
  double beta = atan(c2.slope);
  // Serial.println("alpha");
  // Serial.println(alpha);
  // Serial.println("beta");
  // Serial.println(beta);

  double theta = acos(dot(c1.normal, c2.normal));
  // Serial.println("theta");
  // Serial.println(theta);

  Vector n2_cross_n1 = cross(c2.normal, c1.normal);
  // Serial.println("n2_cross_n1");
  // Serial.println(n2_cross_n1.x);
  // Serial.println(n2_cross_n1.y);
  // Serial.println(n2_cross_n1.z);

  double z = cos(alpha);
  // Serial.println("z");
  // Serial.println(z);

  double beta_z1 = cos(theta-beta);
  double beta_z2 = cos(theta+beta);

  if (((beta_z1 > z) && (beta_z2 > z)) || ((beta_z1 < z) && (beta_z2 < z))) {
    double angle_differences[4] = {
      absolute<double>((theta-beta) - alpha),
      absolute<double>((theta+beta) - alpha),
      absolute<double>((theta-beta) + alpha),
      absolute<double>((theta+beta) + alpha)
    };

    int min_angle_difference_index = 0;
    for (int i=1; i<4; i++){
      if(angle_differences[i] < angle_differences[min_angle_difference_index]) {
        min_angle_difference_index = i;
      }
    }

    float angle_off_n2;
    if (min_angle_difference_index == 0) {
      angle_off_n2 = beta + ((theta-beta)-alpha)/2;
    } else if (min_angle_difference_index == 1) {
      angle_off_n2 = -beta + ((theta+beta)-alpha)/2;
    } else if (min_angle_difference_index == 2) {
      angle_off_n2 = beta + ((theta-beta)+alpha)/2;
    } else if (min_angle_difference_index == 3) {
      angle_off_n2 = -beta + ((theta+beta)+alpha)/2;
    }

    return matrix_multiplication(rotaion_matrix_around_axis(normalize(n2_cross_n1), -angle_off_n2), c2.normal);
  }

  Vector w = Vector(0, sin(theta-beta), cos(theta-beta));
  // Serial.println("w");
  // Serial.println(w.x);
  // Serial.println(w.y);
  // Serial.println(w.z);

  Vector n = Vector(0, sin(theta), cos(theta)); // n2 rotated away from n1 on z-axis
  // Serial.println("n");
  // Serial.println(n.x);
  // Serial.println(n.y);
  // Serial.println(n.z);

  double angle_of_intersection = acos((z-w.y*n.y*n.z-pow(n.z,2)*w.z)/(w.z-w.y*n.y*n.z-pow(n.z,2)*w.z));
  // Serial.println("angle_of_intersection");
  // Serial.println(angle_of_intersection);

  Vector v = matrix_multiplication(rotaion_matrix_around_axis(normalize(n2_cross_n1), beta), c2.normal);
  // Serial.println("v");
  // Serial.println(v.x);
  // Serial.println(v.y);
  // Serial.println(v.z);

  Vector intersection_direction1 = matrix_multiplication(rotaion_matrix_around_axis(c2.normal, angle_of_intersection), v);
  Vector intersection_direction2 = matrix_multiplication(rotaion_matrix_around_axis(c2.normal, -angle_of_intersection), v);
  Serial.println("intersection_direction1");
  Serial.println(intersection_direction1.x);
  Serial.println(intersection_direction1.y);
  Serial.println(intersection_direction1.z);
  Serial.println("intersection_direction2");
  Serial.println(intersection_direction2.x);
  Serial.println(intersection_direction2.y);
  Serial.println(intersection_direction2.z);

  if (intersection_direction1.z > 0) { // we assume our sound is in front of mic array. This assumption will not hold in the future when we fix the 3D issues
      return intersection_direction1;
  } else {
    return intersection_direction2;
  }
}

Vector get_sound_direction(int reads[4][samples], unsigned int timestamps[4][samples]) {
  for (int i=0; i < 6; i++) {
    int pairing[2];
    pairing[0] = mic_pairings[i][0];
    pairing[1] = mic_pairings[i][1];
    double time_offset = cross_correlation_time_offset(reads[pairing[0]], timestamps[pairing[0]], reads[pairing[1]], timestamps[pairing[1]]);
    double distance_between_mics = magnitude(mic_locations[pairing[1]]-mic_locations[pairing[0]]);

    cones[i] = Cone(
      normalize(mic_locations[pairing[1]]-mic_locations[pairing[0]]) * sign(time_offset),
      get_slope_from_time_offset(absolute<double>(time_offset), distance_between_mics)
    );
  }

  Vector cone_intersection_direction_sum = Vector(0,0,0);

  // Serial.println("cone_intersection_direction_sum");
  // Serial.println(cone_intersection_direction_sum.x);
  // Serial.println(cone_intersection_direction_sum.y);
  // Serial.println(cone_intersection_direction_sum.z);

  for (int i=0; i < 5; i++) {
    for (int j=i+1; j < 6; j++) {
      // if ((i == 0) && (j == 1)) {
      if (!((i == 0) && (j == 5)) && !((i == 2) && (j == 3))) { // cones alligned
        Serial.println("cone_intersection_direction_sum");
        Serial.println(i);
        Serial.println(j);
        Vector cone_intersection_direction = get_cone_intersection(cones[i], cones[j]);
        cone_intersection_direction_sum = cone_intersection_direction_sum + cone_intersection_direction;
        // Serial.println(cone_intersection_direction.x);
        // Serial.println(cone_intersection_direction.y);
        // Serial.println(cone_intersection_direction.z);
      }
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