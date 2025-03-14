#include <Arduino.h>
#include <ESP32Servo.h>
#include <iostream>
#include <vector>
#include <stdexcept>

// servo constants
#define SERVO_HERTZ 50
#define SERVO_MIN 500
#define SERVO_MAX 2400
#define NUMBER_OF_SERVOS 3

// TODO: set real min and max constants
// IMPORTANT ::: IMPORTANT
// PLEASE DO THIS OR EVERYTHING WILL BLOW UP!!!!!
// IMPORTANT FIRST STEP AS SOON AS YOU CAN MAKE THINGS RUN
#define MIN_DEGREE 30
#define MAX_DEGREE 70

// serial parameters
#define BAUD_RATE 9600

// Class for the claw fingers
class KrabbyPatty
{
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
    KrabbyPatty(18, 19)};

// zero the claws
bool zero()
{
    bool servosZeroed = true;
    for (int i = 0; i < NUMBER_OF_SERVOS; i++)
    {
        if (krabbyPattyLookUp[i].servo.attached())
        {
            krabbyPattyLookUp[i].servo.write(MIN_DEGREE);
        }
        else
        {
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
String readSerial()
{
    String command = Serial.readString();
    return command;
}

// send a message over serial
// returns true if message successfuly sent
bool writeSerial(String message)
{
    Serial.println(message);
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

    // KrabbyPatty krabbyPatty = krabbyPattyLookUp[servoID];
    // krabbyPatty.servo.write(degree);
    // krabbyPatty.location = degree;

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

// Define the VoltageReading class (formerly Point)
class VoltageReading
{
public:
    int voltage; // Storing the voltage value

    VoltageReading(int voltage = 0) : voltage(voltage) {}

    void display() const
    {
        std::cout << voltage;
    }
};

// Define the VoltageReadingList class
class VoltageReadingList
{
private:
    std::vector<VoltageReading> readings; // The list of voltage readings
    size_t max_size;                      // Maximum number of readings in the list

public:
    // Constructor to initialize with a specific number of readings
    VoltageReadingList(size_t n) : max_size(n) {}

    // Method to add a new voltage reading to the list
    void addReading(const VoltageReading &new_reading)
    {
        if (readings.size() >= max_size)
        {
            // If the list is full, remove the first reading
            readings.erase(readings.begin());
        }
        readings.push_back(new_reading); // Add the new voltage reading
    }

    // Method to display all voltage readings in the list
    void displayReadings() const
    {
        for (const auto &reading : readings)
        {
            reading.display();
            std::cout << " ";
        }
        std::cout << std::endl;
    }

    // Method to get the current size of the list
    size_t getSize() const
    {
        return readings.size();
    }

    // Method to check if the list size is valid
    bool isValidSize() const
    {
        return readings.size() == max_size;
    }

    // Optionally, method to clear the list
    void clear()
    {
        readings.clear();
    }

    // Method to calculate and return the average of the voltage readings
    VoltageReading getAverage() const
    {
        if (readings.empty())
        {
            throw std::runtime_error("Cannot calculate average of an empty list.");
        }

        int sum = 0;
        for (const auto &reading : readings)
        {
            sum += reading.voltage;
        }

        // Calculate average of voltage values
        int avg_voltage = sum / readings.size();

        return VoltageReading(avg_voltage);
    }
};

int NUM_VOLTAGE_READINGS = 20;
VoltageReadingList crab1 = VoltageReadingList(NUM_VOLTAGE_READINGS);
VoltageReadingList crab2 = VoltageReadingList(NUM_VOLTAGE_READINGS);
VoltageReadingList crab3 = VoltageReadingList(NUM_VOLTAGE_READINGS);

// test runner
void loop()
{
    // while (Serial.available()) {
    //     // krabbyPattyIndex: _
    //     String input = Serial.readStringUntil('\n');

    // }
    // Serial.println(krabbyPattyLookUp[1].servo.read());
    // krabbyPattyLookUp[0].servo.write(MIN_DEGREE);
    int x = 50;
    while (1 == 1)
    {
        krabbyPattyLookUp[0].servo.write(50);
        Serial.println(krabbyPattyLookUp[0].servo.attached());
        Serial.println(krabbyPattyLookUp[0].servo.read());

        // Serial.println("working");
        // for (int i = 0; i < 500; i++) {
        //     // delay(10);
        //     Serial.println(analogRead(SERVO_ONE_SENSOR));
        // }
        // servoOne.write(180);
        // for (int i = 0; i < 500; i++) {
        //     // delay(10);
        //     Serial.println(analogRead(SERVO_ONE_SENSOR));
        // }
        // servoOne.write(0);
        // for (int i = 0; i < 500; i++) {
        //     // delay(10);
        //     Serial.println(analogRead(SERVO_ONE_SENSOR));
        // }
    }
}

const int NUM_TILL_CHECK = 5;

// CHECK THE VALUES AND ADJUST THIS
// no point translating to actual voltage values (unneccesary computations)
const int NUM_THRESHOLD = 2000;

// close servo
// TODO: implement logic to run until ampMeter spike
int closeServo(int servoOneAmt, int servoTwoAmt, int servoThreeAmt)
{
    int largestServo = largest(servoOneAmt, servoTwoAmt, servoThreeAmt);
    int servoOneReadings[20];
    int servoTwoReadings[20];
    int servoThreeReadings[20];
    // YASH ABSOLUTELY SUCKS
    //  -LEO

    // increment each servo by one up until closed
    int i = 0;

    bool stop1 = 0;
    bool stop2 = 0;
    bool stop3 = 0;

    for (int i = 0; i < largestServo; i++)
    {
        // check if any have hit
        if (i > NUM_VOLTAGE_READINGS && i == NUM_TILL_CHECK)
        {
            int avg1 = crab1.getAverage().voltage;
            if (avg1 >= NUM_THRESHOLD)
            {
                stop1++;
            }

            int avg2 = crab2.getAverage().voltage;
            if (avg2 >= NUM_THRESHOLD)
            {
                stop2++;
            }

            int avg3 = crab3.getAverage().voltage;
            if (avg1 >= NUM_THRESHOLD)
            {
                stop3++;
            }
        }
        crab1.addReading(analogRead(krabbyPattyLookUp[1].sensor));
        crab2.addReading(analogRead(krabbyPattyLookUp[2].sensor));
        crab3.addReading(analogRead(krabbyPattyLookUp[3].sensor));

        if (krabbyPattyLookUp[1].location < servoOneAmt && stop1 < 4)
        {
            moveServo(1, 1);
        }
        if (krabbyPattyLookUp[2].location < servoTwoAmt && stop2 < 4)
        {
            moveServo(2, 1);
        }
        if (krabbyPattyLookUp[3].location < servoThreeAmt && stop3 < 4)
        {
            moveServo(3, 1);
        }
    }
}