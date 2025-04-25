import serial
import serial.tools
import serial.tools.list_ports

DEVICE = "CP2102 USB to UART Bridge Controller" # TODO: change this if needed

serial_port = None

for p in list(serial.tools.list_ports.comports()):
    desc = p.description.strip()
    if desc == DEVICE:
        serial_port = p.device
        break

if serial_port == None:
    print("Error: Serial Device not found")
    exit(1)

print(serial_port)
ser = serial.Serial(serial_port, baudrate=9600)

while True:
  cmd = input()
  cmd = cmd.strip()
  # ZERO
  # MOVE <ServoID> <Degree>
  ser.write(cmd.encode())

# 0 23
# 1 13
# 2 43