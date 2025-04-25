#include <Arduino.h>
#include <ESP32Servo.h>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <thread>

#define AVERAGE_RESISTANCE_THRESHOLD 1800
#define TRAILING_AVERAGE 50
#define READING_PER_MOVE 10

#define NUMBER_OF_SERVOS 3

#define MIN_DEGREE 0
#define MAX_DEGREE 70

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
    int threshold;
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
        threshold = AVERAGE_RESISTANCE_THRESHOLD;
        readingIndex = 0;
    }
    ClawFinger(int servoPin, int sensorPin, int off, int thres)
    {
        attachServo(servoPin);
        setupSensor(sensorPin);
        location = 0;
        offset = off;
        threshold = thres;
        readingIndex = 0;
    }
    ClawFinger() {}
};

// list of claw fingers
ClawFinger clawFingerLookup[NUMBER_OF_SERVOS] = {
    // servoPin, sensorPin
    ClawFinger(13, 12, 23, 1813),
    ClawFinger(14, 27, 13, 1810),
    ClawFinger(26, 25, 43, 1815)};

void publishClawDegrees() {
    String clawDegreesString = "ClawDegs(";
    for (int i = 0; i < NUMBER_OF_SERVOS; i++) {
        clawDegreesString += String(clawFingerLookup[i].location);
        if (i != NUMBER_OF_SERVOS - 1) {
            clawDegreesString += ",";
        }
    }
    clawDegreesString += ")";
    Serial.println(clawDegreesString);
}

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

void setThresholds()
{
    int readings = 50;
    int readingAvg = 0;
    for (int i = 0; i < NUMBER_OF_SERVOS; i++)
    {
        for (int x = 0; x < readings; x++)
        {
            readingAvg += analogRead(clawFingerLookup[i].sensor);
        }
        readingAvg /= readings;
        clawFingerLookup[i].threshold = readingAvg - BASE_THRESHOLD;
        readingAvg = 0;
    }
}

// TODO: code with a usable zero routine
bool zero()
{
    for (int i = 0; i < NUMBER_OF_SERVOS; i++)
    {
        moveServo(i, 0);
    }

    if (PUBLISH_DEGREES) {
        publishClawDegrees();
    }

    return true;
}

int sign(int x)
{
    if (x > 0)
        return 1;
    if (x < 0)
        return -1;
    return 0;
}

void closeClawToResistance()
{
    bool allResistanceOrZero = false;

    for (int i = 0; i < NUMBER_OF_SERVOS; i++)
    {
        clawFingerLookup[i].resetSensorReadings();
    }

    String servo_0 = "";
    String servo_1 = "";
    String servo_2 = "";

    while (allResistanceOrZero == false)
    {
        allResistanceOrZero = true;

        for (int i = 0; i < NUMBER_OF_SERVOS; i++)
        {

            for (int j = 0; j < READING_PER_MOVE; j++)
            {
                int reading = analogRead(clawFingerLookup[i].sensor);
                clawFingerLookup[i].appendSensorReading(reading);
                if (i == 0)
                {
                    servo_0 += String(reading) + ", ";
                }
                else if (i == 1)
                {
                    servo_1 += String(reading) + ", ";
                }
                else
                {
                    servo_2 += String(reading) + ", ";
                }
            }

            int averageSensorReading = clawFingerLookup[i].averageSensorReadings();
            int newLocation = clawFingerLookup[i].location - 1;
            if (averageSensorReading >= clawFingerLookup[i].threshold && newLocation > 0)
            {
                allResistanceOrZero = false;
                moveServo(i, newLocation);
            }
        }

        if (PUBLISH_DEGREES) {
            publishClawDegrees();
        } else {
            delay(50);
        }
    }

    if (LOG_READOUTS)
    {
        Serial.println("0, " + servo_0);
        Serial.println("1, " + servo_1);
        Serial.println("2, " + servo_2);
    }

    return;
}

bool openClaw()
{
    for (int i = 0; i < NUMBER_OF_SERVOS; i++)
    {
        moveServo(i, MAX_DEGREE);
    }

    if (PUBLISH_DEGREES) {
        publishClawDegrees();
    }

    return true;
}

// runs the operations
bool commandInput(String command)
{
    int spaceOne = command.indexOf(" ");
    String commandId;

    if (spaceOne == -1)
    {
        commandId = command;
    }
    else
    {
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
        int spaceTwo = command.indexOf(" ", spaceOne + 1);
        int servoId = command.substring(spaceOne + 1, spaceTwo).toInt();
        int degree = command.substring(spaceTwo + 1).toInt();
        return moveServo(servoId, degree);
        
        if (PUBLISH_DEGREES) {
            publishClawDegrees();
        }
    }

    return false;
}

// setup stuff
void setup()
{
    Serial.begin(BAUD_RATE);
    setThresholds();
    zero();
}

void loop()
{
    if (Serial.available())
    {
        commandInput(Serial.readString());
    }
    openClaw();
    Serial.println("open");
    delay(5000);
    Serial.println("closing");
    closeClawToResistance();
    Serial.println("closed");
    delay(5000);
}