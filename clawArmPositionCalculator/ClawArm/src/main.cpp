#include <Arduino.h>
#include <ESP32Servo.h>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <thread>

#define AVERAGE_RESISTANCE_THRESHOLD 1800
#define TRAILING_AVERAGE 50
#define READING_PER_MOVE 10

#define NUMBER_OF_SERVOS 1

#define MIN_DEGREE 0
#define MAX_DEGREE 150

#define BAUD_RATE 9600
#define LOG_READOUTS false
#define PUBLISH_DEGREES true

// how far below baseline we consider to be resistance
#define BASE_THRESHOLD 15

// class for claw fingers
class ClawFinger
{
public:
    Servo servo;
    int location;
    int sensor;
    int offset;
    int sensorReadings[TRAILING_AVERAGE];
    int readingIndex;
    bool readingFull = false;
    void resetSensorReadings()
    {
        readingIndex = 0;
        readingFull = false;
    }
    void appendSensorReading(int sensorReading)
    {
        sensorReadings[readingIndex] = sensorReading;
        if ((readingFull == false) && (readingIndex == TRAILING_AVERAGE - 1))
        {
            readingFull = true;
        }
        readingIndex = (readingIndex + 1) % TRAILING_AVERAGE;
    }
    int averageSensorReadings()
    {
        int sumUpTo = readingIndex;
        if (readingFull)
        {
            sumUpTo = TRAILING_AVERAGE;
        }
        int sensorReadingSum = 0;
        for (int i = 0; i < sumUpTo; i++)
        {
            sensorReadingSum += sensorReadings[i];
        }
        return (int)sensorReadingSum / sumUpTo;
    }
    void attachServo(int servoPin)
    {
        servo.attach(servoPin);
    }
    void setupSensor(int sensorPin)
    {
        pinMode(sensorPin, INPUT);
        sensor = sensorPin;
    }
    ClawFinger(int servoPin, int sensorPin)
    {
        attachServo(servoPin);
        setupSensor(sensorPin);
        location = 0;
        offset = 0;
        readingIndex = 0;
    }
    ClawFinger(int servoPin, int sensorPin, int off, int thres)
    {
        attachServo(servoPin);
        setupSensor(sensorPin);
        location = 0;
        offset = off;
        readingIndex = 0;
    }
    ClawFinger() {}
};

enum ClawState
{
    OPEN,
    CLOSED,
};

ClawState currentState;
Servo gripperServo;

// runs the operations
bool commandInput(ClawState command)
{
    Serial.println("Received command: " + String(command));
    switch (command)
    {
    case OPEN:
        gripperServo.write(0);
        currentState = OPEN;
        break;
    case CLOSED:
        gripperServo.write(150);
        currentState = CLOSED;
        break;
    default:
        return true;
    }
    return true;
}



// setup stuff
void setup()
{
    Serial.begin(BAUD_RATE);
    gripperServo.attach(21);
    gripperServo.write(0);
    currentState = OPEN;
    delay(1000);
}

void loop()
{
    if (Serial.available())
    {
        String command = Serial.readString();
        int code = command.toInt();
        if (code < 0 || code > 1)
        {
            Serial.println("Invalid command, please enter 0 for OPEN and 1 for CLOSED");
            return;
        }
        if (code == OPEN) {
            gripperServo.write(0);
        } else if (code == CLOSED) {
            gripperServo.write(120);
        }
    }

    // Send the current state every 10ms
    Serial.println(currentState == OPEN ? "0" : "1");
    delay(10);
}