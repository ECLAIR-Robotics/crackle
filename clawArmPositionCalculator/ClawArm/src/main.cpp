#include <Arduino.h>
#include <ESP32Servo.h>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <thread>

// Maximum value of a 16-bit signed integer
#define INT_MAX_VAL 32767

#define RESISTANCE_THRESHOLD 1800

#define NUMBER_OF_SERVOS 3

#define MIN_DEGREE 0
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
    int offset;
    bool going;
    void attachServo(int servoPin) {
        servo.attach(servoPin);
    }
    void setupSensor(int sensorPin) {
        pinMode(sensorPin, INPUT);
        sensor = sensorPin;
    }
    ClawFinger(int servoPin, int sensorPin)
    {
        attachServo(servoPin);
        setupSensor(sensorPin);
        location = 0;
        offset = 0;
        going = false;
    }
    ClawFinger(int servoPin, int sensorPin, int off)
    {
        attachServo(servoPin);
        setupSensor(sensorPin);
        location = 0;
        offset = off;
        going = false;
    }
    ClawFinger() {}
};

// list of claw fingers
ClawFinger clawFingerLookup[NUMBER_OF_SERVOS] = {
    // servoPin, sensorPin
    ClawFinger(13, 12, 23),
    ClawFinger(14, 27, 13),
    ClawFinger(26, 25, 43)};

void log() {
    String readouts = "";
    for(int i = 0; i < NUMBER_OF_SERVOS; i++)
    {
        readouts += i;
        readouts += " ";
        readouts += analogRead(clawFingerLookup[i].sensor);
        if (i != NUMBER_OF_SERVOS - 1) {
            readouts += ", ";
        }
    } 
    Serial.println(readouts);
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

void closeClawToResistance() {
    bool allResistanceOrZero = false;
    while (allResistanceOrZero == false) {

        allResistanceOrZero = true;

        // log();

        String readouts = "";
        for(int i = 0; i < NUMBER_OF_SERVOS; i++)
        {
            String individual_readout = "";
            individual_readout += i;
            int numberSensorReadings = 10;
            int sensorReadingSum = 0;
            for (int j = 0; j < numberSensorReadings; j++) {
                int reading = analogRead(clawFingerLookup[i].sensor);
                sensorReadingSum += reading;
                individual_readout += ", ";
                individual_readout += reading;
            } 
            Serial.println(individual_readout);
            int averageSensorReading = (int)(sensorReadingSum) / numberSensorReadings;
            int newLocation = clawFingerLookup[i].location - 1;
            if (averageSensorReading >= RESISTANCE_THRESHOLD && newLocation > 0) {
                allResistanceOrZero = false;
                moveServo(i, newLocation);
            }
            readouts += i;
            readouts += " ";
            readouts += averageSensorReading;
            if (i != NUMBER_OF_SERVOS - 1) {
                readouts += ", ";
            }
        }
        Serial.println(readouts);
        delay(50);
    }

    return;
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

// runs all servos to close until resistance or at 0
void simpleClose()
{
    int NUM_SENSOR_READS = 5;
    int DISTANCE_PER_MOVE = 1;
    // make sure all arms are running
    for (int i = 0; i < NUMBER_OF_SERVOS; i++)
    {
        clawFingerLookup[i].going = true;
    }

    bool totalGoing = true;
    while (totalGoing)
    {
        // check each individual finger and totalGoing condition
        totalGoing = false;
        for (int i = 0; i < NUMBER_OF_SERVOS; i++)
        {
            int sensorAvg = 0;
            // check current resistance val
            for (int x = 0; x < NUM_SENSOR_READS; x++)
            {
                sensorAvg += analogRead(clawFingerLookup[i].sensor);
            }
            sensorAvg /= NUM_SENSOR_READS;
            if (clawFingerLookup[i].location <= 0 || sensorAvg < RESISTANCE_THRESHOLD)
            {
                clawFingerLookup[i].going = false;
            }
            totalGoing = totalGoing | clawFingerLookup[i].going;
        }

        // run each servo that still needs to be run
        for (int i = 0; i < NUMBER_OF_SERVOS; i++)
        {
            moveServo(i, DISTANCE_PER_MOVE);
        }
    }
}

void loop()
{
    if (Serial.available()) {
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