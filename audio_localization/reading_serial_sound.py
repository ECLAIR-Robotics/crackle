import serial
import subprocess

ser = serial.Serial('/dev/tty.usbmodem101', baudrate=19200)

line_break = str("1 read, 1 timestamp, 2 read, 2 timestamp, 3 read, 3 timestamp, 4 read, 4 timestamp")

i = 0

output = open(str(i)+".csv", "w+")

while True:
    ser_bytes = str(ser.readline()).strip()[2:-5]
    if ser_bytes == line_break:
        i += 1
        output = open(str(i)+".csv", "w+")
    print(ser_bytes)
    output.write(ser_bytes + "\n")