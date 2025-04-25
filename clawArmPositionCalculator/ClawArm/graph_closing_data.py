import matplotlib.pyplot as plt
import numpy as np
import os

resistance = open("tape_in_the_way.csv", 'r').readlines()
no_resistance = open("no_resistance_new_code.csv", 'r').readlines()

servo_0 = []
servo_1 = []
servo_2 = []

servo_0_resistance = []
servo_1_resistance = []
servo_2_resistance = []

servo_0_trailing_averages_computed = []
servo_1_trailing_averages_computed = []
servo_2_trailing_averages_computed = []

servo_0_resistance_trailing_averages_computed = []
servo_1_resistance_trailing_averages_computed = []
servo_2_resistance_trailing_averages_computed = []

for data_point in no_resistance:
    if data_point[:2] == "0,":
        servo_0 += [int(i) for i in data_point.split(", ")[1:]]
    elif data_point[:2] == "1,":
        servo_1 += [int(i) for i in data_point.split(", ")[1:]]
    elif data_point[:2] == "2,":
        servo_2 += [int(i) for i in data_point.split(", ")[1:]]

for data_point in resistance:
    if data_point[:2] == "0,":
        servo_0_resistance += [int(i) for i in data_point.split(", ")[1:]]
    elif data_point[:2] == "1,":
        servo_1_resistance += [int(i) for i in data_point.split(", ")[1:]]
    elif data_point[:2] == "2,":
        servo_2_resistance += [int(i) for i in data_point.split(", ")[1:]]
    # else:
    #     data_point_split = [int(i.split(" ")[1].strip()) for i in data_point.split(", ")]
    #     servo_0_trailing_averages.append(int(data_point_split[0]))
    #     servo_1_trailing_averages.append(int(data_point_split[1]))
    #     servo_2_trailing_averages.append(int(data_point_split[2]))

for i in range(5, len(servo_0)//10+1):
    servo_0_trailing_averages_computed.append(sum(servo_0[(i-5)*10:i*10])/50)
    servo_1_trailing_averages_computed.append(sum(servo_1[(i-5)*10:i*10])/50)
    servo_2_trailing_averages_computed.append(sum(servo_2[(i-5)*10:i*10])/50)

for i in range(5, len(servo_0_resistance)//10+1):
    servo_0_resistance_trailing_averages_computed.append(sum(servo_0_resistance[(i-5)*10:i*10])/50)
    servo_1_resistance_trailing_averages_computed.append(sum(servo_1_resistance[(i-5)*10:i*10])/50)
    servo_2_resistance_trailing_averages_computed.append(sum(servo_2_resistance[(i-5)*10:i*10])/50)

# plt.plot(list(range(len(servo_0))), servo_0, label = "servo_0")
# plt.plot(list(range(len(servo_1))), servo_1, label = "servo_1")
# plt.plot(list(range(len(servo_2))), servo_2, label = "servo_2")

# plt.plot(list(range(len(servo_0_trailing_averages_computed))), servo_0_trailing_averages_computed, label = "servo_0_trailing_averages_computed")
# plt.plot(list(range(len(servo_1_trailing_averages_computed))), servo_1_trailing_averages_computed, label = "servo_1_trailing_averages_computed")
plt.plot(list(range(len(servo_2_trailing_averages_computed))), servo_2_trailing_averages_computed, label = "servo_2_trailing_averages_computed")

# plt.plot(list(range(len(servo_0_resistance_trailing_averages_computed))), servo_0_resistance_trailing_averages_computed, label = "servo_0_resistance_trailing_averages_computed")
# plt.plot(list(range(len(servo_1_resistance_trailing_averages_computed))), servo_1_resistance_trailing_averages_computed, label = "servo_1_resistance_trailing_averages_computed")
plt.plot(list(range(len(servo_2_resistance_trailing_averages_computed))), servo_2_resistance_trailing_averages_computed, label = "servo_2_resistance_trailing_averages_computed")
    
plt.legend()
plt.show()