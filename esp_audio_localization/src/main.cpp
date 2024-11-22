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

int ind = 0;
bool reading = true;
// bool serialAudioOutput = false;
bool serialAudioOutput = true;

// 240 MHZ 1 cycle per read
int TIME_TO_ANALOG_READ = 50;

void setup()
{
    Serial.begin(111520);
    pinMode(MIC_ZERO, INPUT);
    pinMode(MIC_ONE, INPUT);
    pinMode(MIC_TWO, INPUT);
    pinMode(MIC_THREE, INPUT);

    setCpuFrequencyMhz(240);
}

int absolute(int x)
{
    if (x < 0)
    {
        return -x;
    }
    return x;
}

void loop()
{
    // need first time before rest of reads
    unsigned long timestamp0 = micros() + TIME_TO_ANALOG_READ;
    int read0 = analogRead(MIC_ZERO);
    int read1 = analogRead(MIC_ONE);
    int read2 = analogRead(MIC_TWO);
    int read3 = analogRead(MIC_THREE);

    // calculate rest of the times
    unsigned long timestamp1 = timestamp0 + TIME_TO_ANALOG_READ;
    unsigned long timestamp2 = timestamp1 + TIME_TO_ANALOG_READ;
    unsigned long timestamp3 = timestamp2 + TIME_TO_ANALOG_READ;

    if (reading)
    {
        timestamps0[ind] = timestamp0 - startTimestamp;
        reads0[ind] = read0;
        timestamps1[ind] = timestamp1 - startTimestamp;
        reads1[ind] = read1;
        timestamps2[ind] = timestamp1 - startTimestamp;
        reads2[ind] = read2;
        timestamps3[ind] = timestamp2[ind] + TIME_TO_ANALOG_READ;
        reads3[ind] = read3;

        ind++;
        if (ind >= samples)
        {
            if (serialAudioOutput)
            {
                Serial.println("read 0, timestamp 0, read 1, timestamp 1, read 2, timestamp 2, read 3, timestamp 3");
                for (int i = 0; i < samples; i++)
                {
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
                    // strcat(csvLine, ",");
                    // strcat(csvLine, "\r\n");

                    Serial.println(csvLine);
                    // Serial.print(csvLine);
                }
            }

            reading = false;
        }
    }
    else if (!reading && (absolute(read0 - voltage) > threshold || absolute(read1 - voltage) > threshold || absolute(read2 - voltage) > threshold || absolute(read3 - voltage) > threshold))
    {
        ind = 0;
        reading = true;
        startTimestamp = timestamp0;
    }
}