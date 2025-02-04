from typing import List
import matplotlib.pyplot as plt
import numpy as np
from scipy.fft import fft, fftfreq
import math

# time between polls is 44 microseconds
# 900 polls per sample

BASE_VOLTAGE = 330

def get_time_difference(file_name):
    file = open(file_name, "r")

    one_read = []
    one_timestamps = []
    two_read = []
    two_timestamps = []
    three_read = []
    three_timestamps = []
    four_read = []
    four_timestamps = []

    next(file)
    for line in file:
        line_split = line.strip().split(",")
        one_read.append(int(line_split[0])-BASE_VOLTAGE)
        one_timestamps.append(int(line_split[1]))
        two_read.append(int(line_split[2])-BASE_VOLTAGE)
        two_timestamps.append(int(line_split[3]))
        three_read.append(int(line_split[4])-BASE_VOLTAGE)
        three_timestamps.append(int(line_split[5]))
        four_read.append(int(line_split[6])-BASE_VOLTAGE)
        four_timestamps.append(int(line_split[7]))

    #  left_read = [abs(x) for x in left_read]
    #  right_read = [abs(x) for x in right_read]

    # dvdt_left = np.gradient(left_read, left_timestamps)
    # dvdt_right = np.gradient(right_read, right_timestamps)

    # peaks_left = [False for x in left_timestamps]
    # peaks_right = [False for x in right_timestamps]

    # # assuming dvdt_left contains at least 2 elements
    # for i in range(1, len(dvdt_left)):
    #     peaks_left[i] = sign(dvdt_left[i]) != sign(dvdt_left[i - 1])

    # for i in range(1, len(dvdt_right)):
    #     peaks_right[i] = sign(dvdt_right[i]) != sign(dvdt_right[i - 1])

    # peak_times_left = [left_timestamps[i] for i in range(len(left_timestamps)) if peaks_left[i]]
    # peak_times_right = [right_timestamps[i] for i in range(len(right_timestamps)) if peaks_right[i]]

    plt.plot(np.array(one_timestamps), np.array(one_read), label = "one")
    plt.plot(np.array(two_timestamps), np.array(two_read), label = "two")
    plt.plot(np.array(three_timestamps), np.array(three_read), label = "three")
    plt.plot(np.array(four_timestamps), np.array(four_read), label = "four")
    #  plt.plot(left_timestamps, dvdt_left, label = "dv/dt left")
    #  plt.plot(right_timestamps, dvdt_right, label = "dv/dt right")
    #  plt.vlines(peak_times_left, -10, 10, colors="r", label = "peaks left")
    # plt.vlines(peak_times_right, -10, 10, colors="r", label= "peaks right")
    plt.legend()
    plt.xlabel("Time (ms)")
    plt.ylabel("Amplitude (V)")
    plt.show()

def sign(x):
    return math.copysign(1, x)

print(get_time_difference("testing testing clean.csv"))

# for i in range(1,2):
    #  print(get_time_difference(f"Timestamp Corrected/0 Degrees/{i}.csv"))
    #  print(get_time_difference(f"Timestamp Corrected/45 Degrees Left/{i}.csv"))
    #  print(get_time_difference(f"Timestamp Corrected/90 Degrees Left/{i}.csv"))
    #  print(get_time_difference(f"Timestamp Corrected/45 Degrees Right/{i}.csv"))
    #  print(get_time_difference(f"Timestamp Corrected/90 Degrees Right/{i}.csv"))
