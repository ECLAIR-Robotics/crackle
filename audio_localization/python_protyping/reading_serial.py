import serial
import subprocess

serial_port = None

for p in list(serial.tools.list_ports.comports()):
    if p.description.strip() == "Arduino Mega":
        serial_port = p.device

if serial_port == None:
    print("Arduino Mega not found")

ser = serial.Serial(serial_port, baudrate=19200)


