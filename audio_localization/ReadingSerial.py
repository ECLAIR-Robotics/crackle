import serial
import subprocess

ser = serial.Serial('/dev/tty.usbmodem1301', baudrate=19200)
ser.flushInput()

line_break = str(b'left read, left timestamp, right read, right timestamp\r\n')

i = 0

filename = "90 degrees "

output = open(filename+str(i)+".csv", "w+")

while True:
    ser_bytes = str(ser.readline())
    if ser_bytes == line_break:
        i += 1
        output = open(filename+str(i)+".csv", "w+")
    print(ser_bytes)
    output.write(ser_bytes[2:-5] + "\n")