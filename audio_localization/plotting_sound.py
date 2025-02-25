import math
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import serial

ser = serial.Serial('/dev/tty.usbmodem11301', baudrate=19200, timeout=1)

fig, ax = plt.subplots()

ax.set_xlim([-1, 1])
ax.set_ylim([-1, 1])

drawn = []

intersection_directions = []

def update(frame):
    global drawn
    global intersection_directions
    if ser.in_waiting:
        serial_output = ser.readline().decode('utf-8').strip()

        print(serial_output)

        open_parenthesis = serial_output.find("(") + 1
        end_parenthesis = serial_output.find(")", open_parenthesis)
        vector = [float(i) for i in serial_output[open_parenthesis:end_parenthesis].split(",")]

        if serial_output.startswith("Vector"):

            # if len([direction for direction in intersection_directions if direction[-1] == 'r']) < 7:
            for quiver in drawn:
                quiver.remove()
            drawn = []

            for intersection_direction in intersection_directions:
                drawn.append(ax.quiver(0, 0, intersection_direction[0], intersection_direction[1], color=intersection_direction[3], angles='xy', scale_units='xy', scale = 1, alpha=0.25))
            drawn.append(ax.quiver(0, 0, vector[0], vector[1], color='b', angles='xy', scale_units='xy', scale = 1))

            intersection_directions = []

        else:
            if serial_output.startswith("EdgeIntersectionDirection"):
                vector.append("r")
            elif serial_output.startswith("IntersectionDirection"):
                vector.append("g")
            if len(vector) == 4:
                intersection_directions.append(vector)

ani = FuncAnimation(fig, update, interval=10)

plt.show()