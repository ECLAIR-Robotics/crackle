#include <Arduino.h>

// TODO: change these
const int MIC_ZERO = 33;
const int MIC_ONE = 35;
const int MIC_TWO = 34;
const int MIC_THREE = 32;

const int threshold = 100;
const long voltage = 330;

const int samples = 900;
unsigned long startTimestamp;
int reads0[samples];
unsigned int timestamps0[samples];
int reads1[samples];
unsigned int timestamps1[samples];
int reads2[samples];
unsigned int timestamps2[samples];
int reads3[samples];
unsigned int timestamps3[samples];

int index = 0;
bool reading = true;
bool serialAudioOutput = false;
// bool serialAudioOutput = true;

void setup() {
  Serial.begin(19200);
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
  int read0 = analogRead(MIC_ZERO);
  unsigned long timestamp0 = micros();
  int read1 = analogRead(MIC_ONE);
  unsigned long timestamp1 = micros();
  int read2 = analogRead(MIC_TWO);
  unsigned long timestamp2 = micros();
  int read3 = analogRead(MIC_THREE);
  unsigned long timestamp3 = micros();

  if (reading) {
    reads0[index] = read0;
    timestamps0[index] = timestamp0 - startTimestamp;
    reads1[index] = read1;
    timestamps1[index] = timestamp1 - startTimestamp;
    reads2[index] = read2;
    timestamps2[index] = timestamp2 - startTimestamp;
    reads3[index] = read3;
    timestamps3[index] = timestamp3 - startTimestamp;

    index++;
    if (index >= samples) {
      if (serialAudioOutput) {
        Serial.println("read 0, timestamp 0, read 1, timestamp 1, read 2, timestamp 2, read 3, timestamp 3");
        for (int i = 0; i < samples; i++) {
          char csvLine[400] = "";
          char rstr0[25]; 
          char tstr0[25];
          char rstr1[25]; 
          char tstr1[25];
          char rstr2[25]; 
          char tstr2[25];
          char rstr3[25]; 
          char tstr3[25];
          sprintf(rstr0, "%d", reads0[i]);
          sprintf(tstr0, "%d", timestamps0[i]);
          sprintf(rstr1, "%d", reads1[i]);
          sprintf(tstr1, "%d", timestamps1[i]);
          sprintf(rstr2, "%d", reads2[i]);
          sprintf(tstr2, "%d", timestamps2[i]);
          sprintf(rstr3, "%d", reads3[i]);
          sprintf(tstr3, "%d", timestamps3[i]);

          strcat(csvLine, rstr0);
          strcat(csvLine, ",");
          strcat(csvLine, tstr0);
          strcat(csvLine, ",");
          strcat(csvLine, rstr1);
          strcat(csvLine, ",");
          strcat(csvLine, tstr1);
          strcat(csvLine, ",");
          strcat(csvLine, rstr2);
          strcat(csvLine, ",");
          strcat(csvLine, tstr2);
          strcat(csvLine, ",");
          strcat(csvLine, rstr3);
          strcat(csvLine, ",");
          strcat(csvLine, tstr3);
          strcat(csvLine, ",");

          Serial.println(csvLine);
        }
      }

      reading = false;
    }
  } else if (!reading && (absolute(read0-voltage) > threshold || absolute(read1-voltage) > threshold 
    || absolute(read2-voltage) > threshold || absolute(read3-voltage) > threshold)) {
    index = 0;
    reading = true;
    startTimestamp = timestamp0;
  }
}