import serial
import serial.tools
import serial.tools.list_ports

DEVICE = "CP2102 USB to UART Bridge Controller"

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

output = open("closing_data.csv", "w+")

try:
    while True:
        if ser.in_waiting > 0:
            ser_bytes = str(ser.readline()).strip()[2:-5]
            print(ser_bytes) 
            output.write(ser_bytes + "\n")
except KeyboardInterrupt:
    output.flush()
    output.close()
