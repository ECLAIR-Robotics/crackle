import matplotlib.pyplot as plt
import numpy as np
import os

sensor_readouts = open("2.csv", 'r').readlines()

servo_0 = []
servo_1 = []
servo_2 = []

servo_0_medians = []
servo_1_medians = []
servo_2_medians = []

servo_0_averages = []
servo_1_averages = []
servo_2_averages = []

def getServoMedian(servoArray):
    sorted_arr = sorted(servoArray)
    n = len(sorted_arr)
    mid = n // 2

    if n % 2 == 0:
        # If even, average the two middle numbers
        return (sorted_arr[mid - 1] + sorted_arr[mid]) / 2
    else:
        # If odd, return the middle number
        return sorted_arr[mid]

def getServoAverage(servoArray):
    return sum(servoArray) / len(servoArray)

for data_point in sensor_readouts:
    try:
        data_point_split = [int(i) for i in data_point.split(", ")]
        if data_point_split[0] == 0:
            servo_0_medians.append(getServoMedian(data_point_split[1:]))
            servo_0_averages.append(getServoAverage(data_point_split[1:]))
            servo_0 += data_point_split[1:]
        elif data_point_split[0] == 1:
            servo_1_medians.append(getServoMedian(data_point_split[1:]))
            servo_1_averages.append(getServoAverage(data_point_split[1:]))
            servo_1 += data_point_split[1:]
        else:
            servo_2_medians.append(getServoMedian(data_point_split[1:]))
            servo_2_averages.append(getServoAverage(data_point_split[1:]))
            servo_2 += data_point_split[1:]
    except:
        pass

# plt.plot(list(range(len(servo_0_medians))), servo_0_medians, label = "servo_0_medians")
# plt.plot(list(range(len(servo_0_averages))), servo_0_averages, label = "servo_0_averages")
# plt.legend()
# plt.show()

# plt.plot(list(range(len(servo_1_medians))), servo_1_medians, label = "servo_1_medians")
# plt.plot(list(range(len(servo_1_averages))), servo_1_averages, label = "servo_1_averages")
# plt.legend()
# plt.show()

# plt.plot(list(range(len(servo_2_medians))), servo_2_medians, label = "servo_2_medians")
# plt.plot(list(range(len(servo_2_averages))), servo_2_averages, label = "servo_2_averages")
# plt.legend()
# plt.show()

servo_0_trailing_averages = []
servo_1_trailing_averages = []
servo_2_trailing_averages = []

trailing_average = 50
for i in range(trailing_average, len(servo_0)):
    servo_0_trailing_averages.append(getServoAverage(servo_0[i-trailing_average:i]))
    servo_1_trailing_averages.append(getServoAverage(servo_1[i-trailing_average:i]))
    servo_2_trailing_averages.append(getServoAverage(servo_2[i-trailing_average:i]))

# plt.plot(list(range(len(servo_0))), servo_0, label = "servo_0")
# plt.plot(list(range(len(servo_1))), servo_1, label = "servo_1")
# plt.plot(list(range(len(servo_2))), servo_2, label = "servo_2")

plt.plot(list(range(len(servo_0_trailing_averages))), servo_0_trailing_averages, label = "servo_0_trailing_averages")
plt.plot(list(range(len(servo_1_trailing_averages))), servo_1_trailing_averages, label = "servo_1_trailing_averages")
plt.plot(list(range(len(servo_2_trailing_averages))), servo_2_trailing_averages, label = "servo_2_trailing_averages")
plt.legend()
plt.show()

# plt.plot(list(range(len(servo_0))), servo_0, label = "servo_0_median")
# plt.legend()
# plt.show()

# plt.plot(list(range(len(servo_1))), servo_1, label = "servo_0_avg")
# plt.legend()
# plt.show()

# plt.plot(list(range(len(servo_2))), servo_2, label = "servo_2")
# plt.legend()
# plt.show()


# def plotServoMedians(servo):
#     b = 0
#     aCount = 0
#     a = []
#     for i in range(len(servo) / 10):
#         passThrough = []
#         for a in range(10):
#             passThrough[a] = servo[b]
#             b += 1
#         a[aCount] = getServoMedian(passThrough)
#         aCount += 1
#     plt.plot(list(aCount), a, label = "servo_0_median")
#     plt.legend()
#     plt.show()

# def plotServoAverages(servo):
#     b = 0
#     aCount = 0
#     a = []
#     for i in range(int(len(servo) / 10)):
#         passThrough = []
#         for a in range(10):
#             passThrough[a] = servo[b]
#             b += 1
#         a[aCount] = getServoAverage(passThrough)
#         aCount += 1
#     plt.plot(list(aCount), a, label = "servo_0_avg")
#     plt.legend()
#     plt.show()