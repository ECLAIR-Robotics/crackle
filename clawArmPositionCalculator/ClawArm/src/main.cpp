#include <Arduino.h>
#include <ESP32Servo.h>
#include <map>

// servo pins
#define SERVO_ONE 2
#define SERVO_TWO 4
#define SERVO_THREE 18

// servo constants
#define SERVO_HERTZ 50
#define SERVO_MIN 500
#define SERVO_MAX 2400
// TODO: set real min and max constants
#define MIN_DEGREE 0
#define MAX_DEGREE 180

// Current Sensor Pins
#define SERVO_ONE_SENSOR 15
#define SERVO_TWO_SENSOR 5
#define SERVO_THREE_SENSOR 19

// serial parameters
#define BAUD_RATE 9600

// define servos
Servo servoOne;
Servo servoTwo;
Servo servoThree;

// servo locations
int servoOneLocation;
int servoTwoLocation;
int servoThreeLocation;

// define Servos
std::map<int, Servo> servoLookUp = {
    {1, servoOne},
    {2, servoTwo},
    {3, servoThree}};

// servo locations
std::map<int, int> servoLocations = {
    {1, servoOneLocation},
    {2, servoTwoLocation},
    {3, servoThreeLocation}};

// servo sensors
std::map<int, int> servoSensors = {
    {1, SERVO_ONE_SENSOR},
    {2, SERVO_TWO_SENSOR},
    {3, SERVO_THREE_SENSOR}};

// setup
void setup()
{
    // init servos
    servoOne.attach(SERVO_ONE, SERVO_MIN, SERVO_MAX);
    servoTwo.attach(SERVO_TWO, SERVO_MIN, SERVO_MAX);
    servoThree.attach(SERVO_THREE, SERVO_MIN, SERVO_MAX);

    // serial setup
    Serial.begin(BAUD_RATE);
}

// read current serial message
// returns string on current serial line
String readSerial()
{
    String command = Serial.readString();
    return command;
    pinMode(SERVO_ONE_SENSOR, INPUT);
    pinMode(SERVO_TWO_SENSOR, INPUT);
    pinMode(SERVO_THREE_SENSOR, INPUT);
}

// send a message over serial
// returns true if message successfuly sent
bool writeSerial(String message)
{
    Serial.println(message);
}

// zero the claws
// todo: actually make a zero procedure
bool zero()
{
    servoOneLocation = 0;
    servoTwoLocation = 0;
    servoThreeLocation = 0;
}

// move the servo
// take in servo number and degree
bool moveServo(int servoID, int degree)
{
    // out of bounds
    if (servoID < 1 || servoID > 3 || degree < MIN_DEGREE || degree > MAX_DEGREE)
    {
        return false;
    }

    Servo servo = servoLookUp[servoID];
    servo.write(degree);
    servoLocations[servoID] += degree;

    return true;
}

// get the largest of the three ints
int largest(int servoOneAmt, int servoTwoAmt, int servoThreeAmt)
{
    int largest = servoOneAmt;
    if (servoTwoAmt > largest)
    {
        largest = servoTwoAmt;
    }
    if (servoThreeAmt > largest)
    {
        largest = servoThreeAmt;
    }
    return largest;
}

// close servo
// TODO: implement logic to run until ampMeter spike
int closeServo(int servoOneAmt, int servoTwoAmt, int servoThreeAmt)
{
    int largestServo = largest(servoOneAmt, servoTwoAmt, servoThreeAmt);

    // increment each servo by one up until closed
    for (int i = 0; i < largestServo; i++)
    {
        if (servoLocations[0] < servoOneAmt)
        {
            moveServo(1, 1);
        }
        if (servoLocations[1] < servoTwoAmt)
        {
            moveServo(2, 1);
        }
        if (servoLocations[2] < servoThreeAmt)
        {
            moveServo(3, 1);
        }
    }
}

// runs the operations
bool runOp(String command)
{
    // find index of end of the first command
    int spaceOne = command.indexOf(" ");
    String commandId;

    // no-parameter command
    if (spaceOne == -1)
    {
        commandId = command;
    }
    // command has parameters
    else
    {
        commandId = command.substring(0, spaceOne);
    }

    // zero axis
    if (commandId.equals("Z"))
    {
        zero();
        return true;
    }

    // move servo by fixed amount
    if (commandId.equals("MOV"))
    {
        int spaceTwo = command.indexOf(" ", spaceOne);
        int servoId = command.substring(spaceOne, spaceTwo).toInt();
        int degree = command.substring(spaceTwo).toInt();
        return moveServo(servoId, degree);
    }

    // close servo claws

    // command id not recognized
    else
    {
        return false;
    }
}

// test runner
void loop()
{
    int x = 10;
    while (1 == 1)
    {
        Serial.println(analogRead(SERVO_ONE_SENSOR));
        servoOne.write(x);
        Serial.println(analogRead(SERVO_ONE_SENSOR));
        servoOne.write(-x);
    }
}
