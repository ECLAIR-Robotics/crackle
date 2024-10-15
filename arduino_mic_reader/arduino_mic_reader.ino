#include <Arduino.h>

const int voltage = 500; // 5.5 volts

const int lower_bound = 100;
const int higher_bound = 450;

const int MIC_RIGHT = A0;
const int MIC_LEFT = A15;

void setup() {
  Serial.begin(9600);
  pinMode(MIC_RIGHT, INPUT);
  pinMode(MIC_LEFT, INPUT);
}

bool left_mic_listening = false;
int left_max_read = 0;
long left_max_time_stamp;

bool right_mic_listening = false;
int right_max_read = 0;
long right_max_time_stamp;

int absolute(int x) {
  if (x < 0) {
    return -x;
  }
  return x;
}

void loop() {
  int left_read = absolute(analogRead(MIC_LEFT)-voltage);
  if (!left_mic_listening && left_read > higher_bound) {
    left_mic_listening = true;
    left_max_read = left_read;
    left_max_time_stamp = micros();
  }
  if (left_mic_listening) {
    if (left_read > left_max_read) {
      left_max_read = left_read;
      left_max_time_stamp = micros();
    } else if (left_read < lower_bound) {
      left_mic_listening = false;
    }
  }

  int right_read = absolute(analogRead(MIC_RIGHT)-voltage);
  if (!right_mic_listening && right_read > higher_bound) {
    right_mic_listening = true;
    right_max_read = right_read;
    right_max_time_stamp = micros();
  }
  if (right_mic_listening) {
    if (right_read > right_max_read) {
      right_max_read = right_read;
      right_max_time_stamp = micros();
    } else if (right_read < lower_bound) {
      right_mic_listening = false;
    }
  }

  if (!left_mic_listening && !right_mic_listening && left_max_time_stamp != 0 && right_max_time_stamp != 0 && absolute(left_max_time_stamp-right_max_time_stamp) < 1000) {
    Serial.println(absolute(left_max_time_stamp-right_max_time_stamp));
    left_max_time_stamp = 0;
    right_max_time_stamp = 0;
  }
}