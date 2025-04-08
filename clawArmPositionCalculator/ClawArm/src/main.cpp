#include <Arduino.h>
#include <ESP32Servo.h>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <thread>

// Maximum value of a 16-bit signed integer
#define INT_MAX_VAL 32767

#define RESISTANCE_THRESHOLD 1780

#define NUMBER_OF_SERVOS 3

#define MIN_DEGREE 0
#define MAX_DEGREE 90

// serial parameters
#define BAUD_RATE 9600

// class for claw fingers
class ClawFinger
{
public:
    Servo servo;
    int location;
    int sensor;
    int offset;
    ClawFinger(int servoPin, int sensorPin)
    {
        servo.attach(servoPin);
        location = 0;
        pinMode(sensorPin, INPUT);
        sensor = sensorPin;
        offset = 0;
    }
    ClawFinger(int servoPin, int sensorPin, int off)
    {
        ClawFinger(servoPin, sensorPin);
        offset = offset;
    }
    ClawFinger() {}
};

// list of claw fingers
ClawFinger clawFingerLookup[NUMBER_OF_SERVOS] = {
    ClawFinger(13, 12),
    ClawFinger(27, 14),
    ClawFinger(26, 25)};

// moves an individual servo w/out reacting to resistance
bool moveServo(int servoID, int degree)
{
    // degree out of bounds
    if (servoID < 0 || servoID >= NUMBER_OF_SERVOS || degree < MIN_DEGREE || degree > MAX_DEGREE)
    {
        return false;
    }
    clawFingerLookup[servoID].servo.write(degree + clawFingerLookup[servoID].offset);
    clawFingerLookup[servoID].location = degree;
    return true;
}

// TODO: code with a usable zero routine
bool zero()
{
    for(int i = 0; i < NUMBER_OF_SERVOS; i++)
    {
        moveServo(i, 0);
    }
    return true;
}

int sign(int x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

int moveServoToResistanceRecursive(int servoID, int degree, int finalDegree)
{
    if (degree == finalDegree || degree < MIN_DEGREE || degree > MAX_DEGREE) {
        return clawFingerLookup[servoID].location;
    }
    int numberSensorReadings = 5;
    int sensorReadingSum = 0;
    for (int i = 0; i < numberSensorReadings; i++) {
        sensorReadingSum += analogRead(clawFingerLookup[servoID].sensor);
    } 
    int averageSensorReading = (int)(sensorReadingSum) / numberSensorReadings;
    if (averageSensorReading < RESISTANCE_THRESHOLD) {
        return clawFingerLookup[servoID].location;
    }

    bool servoMoved = moveServo(servoID, degree); 
    if (servoMoved == false) {
        return clawFingerLookup[servoID].location;
    }
    
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    return moveServoToResistanceRecursive(servoID, degree + sign(finalDegree-clawFingerLookup[servoID].location)*1, finalDegree);
}

int moveServoToResistance(int servoID, int finalDegree) {
    if (clawFingerLookup[servoID].location == finalDegree) {
        return finalDegree;
    }
    return moveServoToResistanceRecursive(servoID, clawFingerLookup[servoID].location + sign(finalDegree-clawFingerLookup[servoID].location)*1, finalDegree);
}

// moveServoToResistance to 0 for all servos
bool closeClaw()
{
    std::thread threads[NUMBER_OF_SERVOS];
    for(int i = 0; i < NUMBER_OF_SERVOS; i++)
    {
        threads[i] = std::thread(moveServoToResistance, i, 0);
    }
    for(int i = 0; i < NUMBER_OF_SERVOS; i++)
    {
        threads[i].join();
    }
    return true;
}

bool openClaw()
{
    for(int i = 0; i < NUMBER_OF_SERVOS; i++)
    {
       moveServo(i, MAX_DEGREE);
    }
    return true;
}

// runs the operations
bool commandInput(String command)
{
    int spaceOne = command.indexOf(" ");
    String commandId;

    if (spaceOne == -1) {
        commandId = command;
    } else {
        commandId = command.substring(0, spaceOne);
    }
    commandId.toLowerCase();

    // ZERO
    if (commandId.equals("zero"))
    {
        zero();
        return true;
    }

    // MOVE <ServoID> <Degree>
    if (commandId.equals("move"))
    {
        int spaceTwo = command.indexOf(" ", spaceOne+1);
        int servoId = command.substring(spaceOne+1, spaceTwo).toInt();
        int degree = command.substring(spaceTwo+1).toInt();
        return moveServo(servoId, degree);
    }

    return false;
}

// setup stuff
void setup()
{
    Serial.begin(BAUD_RATE);
    zero();
}

void loop()
{
    if (Serial.available()) {
        commandInput(Serial.readString());
    }
    openClaw();
    Serial.println("open");
    delay(10000);
    Serial.println("closing");
    closeClaw();
    Serial.println("closed");
    delay(10000);
}