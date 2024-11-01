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

    left_read = []
    left_timestamps = []
    right_read = []
    right_timestamps = []

    next(file)
    for line in file:
        line_split = line.strip().split(",")
        left_read.append(int(line_split[0])-BASE_VOLTAGE)
        left_timestamps.append(int(line_split[1]))
        right_read.append(int(line_split[2])-BASE_VOLTAGE)
        right_timestamps.append(int(line_split[3]))

    #  left_read = [abs(x) for x in left_read]
    #  right_read = [abs(x) for x in right_read]

    dvdt_left = np.gradient(left_read, left_timestamps)
    dvdt_right = np.gradient(right_read, right_timestamps)

    peaks_left = [False for x in left_timestamps]
    peaks_right = [False for x in right_timestamps]

    # assuming dvdt_left contains at least 2 elements
    for i in range(1, len(dvdt_left)):
        peaks_left[i] = sign(dvdt_left[i]) != sign(dvdt_left[i - 1])

    for i in range(1, len(dvdt_right)):
        peaks_right[i] = sign(dvdt_right[i]) != sign(dvdt_right[i - 1])

    peak_times_left = [left_timestamps[i] for i in range(len(left_timestamps)) if peaks_left[i]]
    peak_times_right = [right_timestamps[i] for i in range(len(right_timestamps)) if peaks_right[i]]

    #  plt.plot(np.array(left_timestamps), np.array(left_read), label = "left")
    plt.plot(np.array(right_timestamps), np.array(right_read), label = "right")
    #  plt.plot(left_timestamps, dvdt_left, label = "dv/dt left")
    #  plt.plot(right_timestamps, dvdt_right, label = "dv/dt right")
    #  plt.vlines(peak_times_left, -10, 10, colors="r", label = "peaks left")
    plt.vlines(peak_times_right, -10, 10, colors="r", label= "peaks right")
    plt.legend()
    plt.xlabel("Time (ms)")
    plt.ylabel("Amplitude (V)")
    plt.show()

def sign(x):
    return math.copysign(1, x)

for i in range(1,2):
    #  print(get_time_difference(f"Timestamp Corrected/0 Degrees/{i}.csv"))
    #  print(get_time_difference(f"Timestamp Corrected/45 Degrees Left/{i}.csv"))
    #  print(get_time_difference(f"Timestamp Corrected/90 Degrees Left/{i}.csv"))
    print(get_time_difference(f"Timestamp Corrected/45 Degrees Right/{i}.csv"))
    #  print(get_time_difference(f"Timestamp Corrected/90 Degrees Right/{i}.csv"))
