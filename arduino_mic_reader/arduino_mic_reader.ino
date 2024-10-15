#include <Arduino.h>

const int voltage = 330; // 3.3 volts

const int lower_bound = 150;
const int higher_bound = 300;

const int MIC_RIGHT = A0;
const int MIC_LEFT = A15;

void setup() {
  Serial.begin(19200);
  ADCSRA = (ADCSRA & 0xF8) | 0x04; // Prescaler of 16
  ADCSRA &= ~(1 << ADATE); // Disable auto triggering
  pinMode(MIC_RIGHT, INPUT);
  pinMode(MIC_LEFT, INPUT);
}

bool left_mic_listening = false;
int left_max_read = 0;
long left_max_time_stamp;
int left_last_read;
int left_second_last_read;

bool right_mic_listening = false;
int right_max_read = 0;
long right_max_time_stamp;
int right_last_read;
int right_second_last_read;

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
    } else if (left_last_read > left_second_last_read && left_read < left_last_read && left_last_read < lower_bound) {
      left_mic_listening = false;
    }
  }
  left_second_last_read = left_last_read;
  left_last_read = left_read;

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
    } else if (right_last_read > right_second_last_read && right_read < right_last_read && right_read < lower_bound) {
      right_mic_listening = false;
    }
  }
  right_second_last_read = right_last_read;
  right_last_read = right_read;

  // Serial.print("Mic_Left:");
  // Serial.print(analogRead(MIC_LEFT));
  // Serial.print(", ");

  // Serial.print("Mic_Right:");
  // Serial.println(analogRead(MIC_RIGHT));

  if (!left_mic_listening && !right_mic_listening && left_max_time_stamp != 0 && right_max_time_stamp != 0 && absolute(left_max_time_stamp-right_max_time_stamp) < 1000) {
    Serial.println(left_max_time_stamp-right_max_time_stamp);
    left_max_time_stamp = 0;
    right_max_time_stamp = 0;
  }
}