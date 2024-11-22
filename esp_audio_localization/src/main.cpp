#include <Arduino.h>
#include <driver/i2s.h>
#include "esp_adc_cal.h"
#include "freertos/FreeRTOS.h"

#define DEFAULT_VREF 1100
// take 2^POW_SAMPLES samples
const uint32_t POW_SAMPLES = 0;

static esp_adc_cal_characteristics_t* adc_chars;
static const adc_bits_width_t WIDTH = ADC_WIDTH_BIT_12;
static const adc_atten_t ATTEN = ADC_ATTEN_DB_12;
static const adc_unit_t UNIT = ADC_UNIT_1;

static const adc1_channel_t CHANNEL_ZERO = ADC1_CHANNEL_5;
static const adc1_channel_t CHANNEL_ONE = ADC1_CHANNEL_7;
static const adc1_channel_t CHANNEL_TWO = ADC1_CHANNEL_6;
static const adc1_channel_t CHANNEL_THREE = ADC1_CHANNEL_4;

// TODO: change these
// const int MIC_ZERO = 33;
// const int MIC_ONE = 35;
// const int MIC_TWO = 34;
// const int MIC_THREE = 32;

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

void setup() {
  adc1_config_width(WIDTH);
  adc1_config_channel_atten(CHANNEL_ZERO, ATTEN);
  adc1_config_channel_atten(CHANNEL_ONE, ATTEN);
  adc1_config_channel_atten(CHANNEL_TWO, ATTEN);
  adc1_config_channel_atten(CHANNEL_THREE, ATTEN);

  adc_chars = (esp_adc_cal_characteristics_t *) calloc(1, sizeof(esp_adc_cal_characteristics_t));
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(UNIT, ATTEN, WIDTH, DEFAULT_VREF, adc_chars);

  setCpuFrequencyMhz(240);

  Serial.begin(111520);
}

int absolute(int x) {
  if (x < 0) {
    return -x;
  }
  return x;
}

uint32_t readVoltage(adc1_channel_t channel) {
  uint32_t adc_reading = 0;

  uint32_t num_samples = 1 << POW_SAMPLES;

  for (int i = 0; i < num_samples; i++) {
    adc_reading += adc1_get_raw(channel);
  }

  // divide by 64
  adc_reading >>= POW_SAMPLES;
  uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);

  return voltage;
}

void loop() {
    // need first time before rest of reads
    unsigned long timestamp0 = micros() + TIME_TO_ANALOG_READ;
    int read0 = readVoltage(CHANNEL_ZERO);
    int read1 = readVoltage(CHANNEL_ONE);
    int read2 = readVoltage(CHANNEL_TWO);
    int read3 = readVoltage(CHANNEL_THREE);

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
        timestamps2[ind] = timestamp2 - startTimestamp;
        reads2[ind] = read2;
        timestamps3[ind] = timestamp3 - startTimestamp;
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
    else if (!reading && (absolute(read0 - voltage) > threshold || absolute(read1 - voltage) > threshold || absolute(read2 - voltage) > threshold || absolute(read3 - voltage) > threshold)) {
        ind = 0;
        reading = true;
        startTimestamp = timestamp0;
    }
}
