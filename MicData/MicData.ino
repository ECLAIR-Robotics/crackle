#include <Arduino.h>

const int MIC_LEFT = A0;
const int MIC_RIGHT = A15;

const int threshold = 100;
const int voltage = 330;

const int listLength = 900;
int leftReads[listLength];
int leftTimestamps[listLength];
int rightReads[listLength];
int rightTimestamps[listLength];
int index = 0;
bool reading = true;

void setup() {
  Serial.begin(19200);
  ADCSRA = (ADCSRA & 0xF8) | 0x04; // Set Analogue Read Rate
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
  int leftTimestamp = micros();
  int rightRead = analogRead(MIC_RIGHT);
  int rightTimestamp = micros();
  if (!reading && (absolute(leftRead-voltage) > threshold || absolute(rightRead-voltage) > threshold)) {
    index = 0;
    reading = true;
  }
  if (reading) {
    leftReads[index] = leftRead;
    leftTimestamps[index] = leftTimestamp;
    rightReads[index] = rightRead;
    rightTimestamps[index] = rightTimestamp;
    index++;
    if (index >= listLength) {
      Serial.println("left read, left timestamp, right read, right timestamp");
      for (int i = 0; i < listLength; i++) {
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
      reading = false;
    }
  }
}
