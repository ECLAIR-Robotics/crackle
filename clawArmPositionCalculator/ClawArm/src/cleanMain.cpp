#include <Arduino.h>
#include <ESP32Servo.h>
#include <iostream>
#include <vector>
#include <stdexcept>

// Maximum value of a 16-bit signed integer
int INT_MAX_VAL = 32767;

// servo constants
#define SERVO_HERTZ 50
#define SERVO_MIN 500
#define SERVO_MAX 2400
#define NUMBER_OF_SERVOS 3

// how many degrees we write between sensor reads
#define MANUAL_STEP_CHECK_AMOUNT = 5;

// TODO: set real min and max constants
// IMPORTANT ::: IMPORTANT
// PLEASE DO THIS OR EVERYTHING WILL BLOW UP!!!!!
// IMPORTANT FIRST STEP AS SOON AS YOU CAN MAKE THINGS RUN
#define MIN_DEGREE 30
#define MAX_DEGREE 70

// serial parameters
#define BAUD_RATE 9600

// class for claw fingers
class ClawFinger
{
public:
    Servo servo;
    int location;
    int sensor;
    ClawFinger(int servoPin, int sensorPin)
    {
        servo.attach(servoPin);
        location = 0;
        pinMode(sensorPin, INPUT);
        sensor = sensorPin;
    }
    ClawFinger() {}
};

// list of claw fingers
ClawFinger clawFingerLookup[NUMBER_OF_SERVOS] = {
    ClawFinger(13, 12),
    ClawFinger(4, 5),
    ClawFinger(18, 19)};

// TODO: code with a usable zero routine
bool zeroServos()
{
    // assume they are already in correct position rn
    return true;
}

// setup stuff
void setup()
{
    Serial.begin(BAUD_RATE);
    zeroServos();
}

// servo control methods:

// moves an individual servo w/out reacting to resistance
bool moveServo(int servoID, int degree)
{
    // degree out of bounds
    if (degree < MIN_DEGREE || degree > MAX_DEGREE)
    {
        return false;
    }
    clawFingerLookup[servoID].servo.write(degree);
    clawFingerLookup[servoID].location += degree;
    return true;
}

// move an individual servo, stopping if resistance encountered
// return -1 if degree out of bounds
// return 0 if encountered resistance (writes correct position to location in this case)
// return 1 if moved till degree with no resistance encountered
int moveServoStopOnResistance(int servoID, int degree)
{
    // degree out of bounds
    if (degree < MIN_DEGREE || degree > MAX_DEGREE)
    {
        return -1;
    }

    // TODO: write method body
}

// close the whole claw together
// all items in returnVals set to INT_MAX_VAL  if any deg out of bounds
// else return vals = sent rotation amount - actual rotation amount until resistance
// if any particular claw went beyond threshold in delta, its position set to INT_MAX_VAL
void closeClaw(int servoOneDeg, int servoTwoDeg, int servoThreeDeg, int returnVals[3])
{
    // degree out of bounds
    if (servoOneDeg < MIN_DEGREE || servoOneDeg > MAX_DEGREE || servoTwoDeg < MIN_DEGREE || servoTwoDeg > MAX_DEGREE || servoThreeDeg < MIN_DEGREE || servoThreeDeg > MAX_DEGREE)
    {
        returnVals[0] = INT_MAX_VAL;
        returnVals[1] = INT_MAX_VAL;
        returnVals[2] = INT_MAX_VAL;
    }

    // TODO: make method body
}