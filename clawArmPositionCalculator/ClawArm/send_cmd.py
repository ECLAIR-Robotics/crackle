import serial
import serial.tools
import serial.tools.list_ports

serial_port = None

for p in list(serial.tools.list_ports.comports()):
    desc = p.description.strip()
    if "CP2102 USB to UART Bridge Controller" in desc:
        serial_port = p.device
        break

if serial_port == None:
    print("ESP32 not found")
    exit(1)

print(serial_port)
ser = serial.Serial(serial_port, baudrate=9600)

while True:
  cmd = input()
  cmd = cmd.strip()
  # ZERO
  # MOVE <ServoID> <Degree>
  # OPEN
  # CLOSE
  ser.write(cmd.encode())