import matplotlib.pyplot as plt
import numpy as np
import os

closing_data = open("closing_data.csv", 'r').readlines()

servo_0 = []
servo_1 = []
servo_2 = []

for data_point in closing_data:
    data_point_split = [int(i.split(" ")[1].strip()) for i in data_point.split(", ")]
    servo_0.append(data_point_split[0])
    servo_1.append(data_point_split[1])
    servo_2.append(data_point_split[2])

plt.plot(list(range(len(closing_data))), servo_0, label = "servo_0")
plt.plot(list(range(len(closing_data))), servo_1, label = "servo_1")
plt.plot(list(range(len(closing_data))), servo_2, label = "servo_2")
    
plt.legend()
plt.show()