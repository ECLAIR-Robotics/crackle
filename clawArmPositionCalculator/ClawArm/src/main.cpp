#include <Arduino.h>
#include <ESP32Servo.h>

// servo constants
#define SERVO_HERTZ 50
#define SERVO_MIN 500
#define SERVO_MAX 2400
#define NUMBER_OF_SERVOS 3

// TODO: set real min and max constants
// IMPORTANT ::: IMPORTANT
// PLEASE DO THIS OR EVERYTHING WILL BLOW UP!!!!!
// IMPORTANT FIRST STEP AS SOON AS YOU CAN MAKE THINGS RUN
#define MIN_DEGREE 0
#define MAX_DEGREE 180

// serial parameters
#define BAUD_RATE 9600

// Class for the claw fingers
class KrabbyPatty {
    public:
        Servo servo;
        int location;
        int sensor;
        KrabbyPatty(int servoPin, int sensorPin)
        {
            servo.attach(servoPin);
            location = 0;
            pinMode(sensorPin, INPUT);
            sensor = sensorPin;
        }
        KrabbyPatty() {}
};

// servo list
KrabbyPatty krabbyPattyLookUp[NUMBER_OF_SERVOS] = {
    KrabbyPatty(13, 12),
    KrabbyPatty(4, 5),
    KrabbyPatty(18, 19)
};

// zero the claws
bool zero()
{
    bool servosZeroed = true;
    for (int i = 0; i < NUMBER_OF_SERVOS; i++)
    {
        if (krabbyPattyLookUp[i].servo.attached()) {
            krabbyPattyLookUp[i].servo.write(MIN_DEGREE);
        } else {
            servosZeroed = false;
        }
    }
    return servosZeroed;
}

// setup
void setup()
{
    Serial.begin(BAUD_RATE);

    zero();
}

// read current serial message
// returns string on current serial line
// String readSerial()
// {
//     String command = Serial.readString();
//     return command;
// }

// // send a message over serial
// // returns true if message successfuly sent
// bool writeSerial(String message)
// {
//     Serial.println(message);
// }

// move the servo
// take in servo number and degree
bool moveServo(int servoID, int degree)
{
    if (servoID < 0 || servoID >= NUMBER_OF_SERVOS || degree < MIN_DEGREE || degree > MAX_DEGREE) {
        return false;
    }

    krabbyPattyLookUp[servoID].servo.write(degree);
    return true;
}

// get the largest of the three ints
// int largest(int servoOneAmt, int servoTwoAmt, int servoThreeAmt)
// {
//     int largest = servoOneAmt;
//     if (servoTwoAmt > largest)
//     {
//         largest = servoTwoAmt;
//     }
//     if (servoThreeAmt > largest)
//     {
//         largest = servoThreeAmt;
//     }
//     return largest;
// }

// close servo
// TODO: implement logic to run until ampMeter spike
int closeServo(int servoOneAmt, int servoTwoAmt, int servoThreeAmt)
{
    // int largestServo = largest(servoOneAmt, servoTwoAmt, servoThreeAmt);
    // int servoOneReadings[20];
    // int servoTwoReadings[20];
    // int servoThreeReadings[20];

    // // increment each servo by one up until closed
    // for (int i = 0; i < largestServo; i++)
    // {
    //     // if (krabbyPattyLookUp[1].location < servoOneAmt)
    //     // {
    //     //     moveServo(1, 1);
    //     // }
    //     // if (krabbyPattyLookUp[2].location < servoTwoAmt)
    //     // {
    //     //     moveServo(2, 1);
    //     // }
    //     // if (krabbyPattyLookUp[3].location < servoThreeAmt)
    //     // {
    //     //     moveServo(3, 1);
    //     // }
    // }
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

int sensor_readings[10];

// test runner
void loop()
{
    while (Serial.available()) {
        commandInput(Serial.readString());
    }
    // Serial.println(analogRead(krabbyPattyLookUp[0].sensor));
    // int sensor_readout = kp.sensor.analogRead();
    // Serial.println(krabbyPattyLookUp[0].sensor.analogRead());
    // delay(5000);
    while (1 == 1) {
        // Serial.println(analogRead(krabbyPattyLookUp[0].sensor));
        Serial.println(krabbyPattyLookUp[0].servo.read());
        // Serial.println(krabbyPattyLookUp[0].servo.readTimerWidth());
        moveServo(0, 50);
    }
    // delay(5000);
    // moveServo(0, 100);
}

#define sensorThreshold 300