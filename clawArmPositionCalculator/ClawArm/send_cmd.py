import serial
import serial.tools
import serial.tools.list_ports
import time

DEVICE = "CP2102 USB to UART Bridge Controller" # TODO: change this if needed

serial_port = None

for p in list(serial.tools.list_ports.comports()):
    desc = p.description.strip()
    print(desc)
    if desc == DEVICE:
        serial_port = p.device
        break

if serial_port == None:
    print("Error: Serial Device not found")
    exit(1)

print(serial_port)
ser = serial.Serial(serial_port, baudrate=9600)

def send_command(command: str):
  ser.write(command.encode())

while True:
  cmd = input()
  cmd = cmd.strip()
  try:
    # TODO: change the commands if needed
    if cmd == "1":
      send_command("move 0 30")
    elif cmd == "2":
      send_command("move 0 90")
    elif cmd == "0":
      send_command("zero")
  except:
    break