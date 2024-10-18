#include <Arduino.h>

const int VOLTAGE = 330; // 3.3 volts

// Mic Setup
const int LEFT_MIC = A0;
const int RIGHT_MIC = A15;

// Avg calculator setup
const double FIRST_REGISTER_RATIO = 1.75;  // how far over sx first heard sound must be
const double SECOND_REGISTER_RATIO = 1.25; // how far over sx second heard sound must be
double leftAvg = 0;                        // avg heard sound of leftMic, update as progresses
double rightAvg = 0;                       // avg heard sound of rightMic, update as progresses
const int NUM_READS_PER_AVG = 150;         // how many reads must be made to update sound average
double leftHistory[NUM_READS_PER_AVG];     // history for next avg calc on left mic
double rightHistory[NUM_READS_PER_AVG];    // history for next avg calc on right mic
int numReadsMade = 0;                      // how many reads have been made to this point

// rate at which signal is sent over USB
const int BAUDRATE = 9600;

const unsigned long WAIT_TIME = 500; // how long we will freeze the program to wait for sound

void setup()
{
    Serial.begin(BAUDRATE);
    pinMode(LEFT_MIC, INPUT);
    pinMode(RIGHT_MIC, INPUT);
    calibrateMics();
    ADCSRA = (ADCSRA & 0xF8) | 0x04; // Set Analogue Read Rate
    ADCSRA &= ~(1 << ADATE); // Disable auto triggering
}

// get an initial average to base everything else off of
// ideally run this with average background noise and no distrubrance you would want detected
void calibrateMics()
{
    for (int i = 0; i < NUM_READS_PER_AVG; i++)
    {
        double leftSound = analogRead(LEFT_MIC);
        double rightSound = analogRead(RIGHT_MIC);
        updateAverages(leftSound, rightSound);
    }
}

// calculates avg of values in an array
double getAverage(double vals[])
{
    double avg = 0;
    for (int i = 0; i < sizeof(vals); i++)
    {
        avg += vals[i];
    }
    return avg / sizeof(vals);
}

// reset the historys
void resetHist()
{
    for (int i = 0; i < NUM_READS_PER_AVG; i++)
    {
        leftHistory[i] = 0;
        rightHistory[i] = 0;
    }
}

// updates left and right mic averages
void updateAverages(double leftVal, double rightVal)
{
    // calculate avgs / reset counter for cycle: takes many computations so do
    // infrequently. Frequency determined by NUM_READS_PER_AVG. NOTE: lower frequency
    // means more computations each time this happens but should be neg. increase
    if (numReadsMade == NUM_READS_PER_AVG)
    {
        leftAvg = getAverage(leftHistory);
        rightAvg = getAverage(rightHistory);
        resetHist();
        numReadsMade = 0;
    }

    // add vals to next calculation of avg
    leftHistory[numReadsMade] = leftVal;
    rightHistory[numReadsMade] = rightVal;
    numReadsMade++;
}

// each time we are looking for a deltaTime
// return time delta if applicable
//      positive val if right first
//      negative val if left first
//      WAIT_TIME + 1 if trash data
unsigned long getAudioDelta()
{
    unsigned long startTime = micros();
    while (micros() - startTime < WAIT_TIME)
    {
        // right mic heard first
        if (analogRead(RIGHT_MIC) > rightAvg * FIRST_REGISTER_RATIO)
        {
            unsigned long firstRead = micros();
            while (int(micros() - firstRead) < WAIT_TIME)
            {
                if (analogRead(LEFT_MIC) > leftAvg * SECOND_REGISTER_RATIO)
                {
                    return micros() - firstRead;
                }
            }
            return WAIT_TIME + 1;
        }

        // left mic heard first
        if (analogRead(LEFT_MIC) > leftAvg * FIRST_REGISTER_RATIO)
        {
            unsigned long firstRead = micros();
            while (micros() - firstRead < WAIT_TIME)
            {
                if (analogRead(RIGHT_MIC) > rightAvg * SECOND_REGISTER_RATIO)
                {
                    return micros() - firstRead;
                }
            }
            return WAIT_TIME + 1;
        }
    }

    return WAIT_TIME + 1;
}

void loop()
{
    // get the delta
    unsigned long delta = getAudioDelta();

    // only waste time with serial when data is useful
    if (delta != WAIT_TIME + 1 || delta == 40)
    {
        Serial.println(delta - 40);
    }

    // take a sample for avg and update if needed
    double leftSound = analogRead(LEFT_MIC);
    double rightSound = analogRead(RIGHT_MIC);
    updateAverages(leftSound, rightSound);
}
