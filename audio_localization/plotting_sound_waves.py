from typing import List
import matplotlib.pyplot as plt
import numpy as np
from scipy.fft import fft, fftfreq
import math

# time between polls is 44 microseconds
# 900 polls per sample

BASE_VOLTAGE = 330

def plot_sound_waves(file_name):
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
        if ("EdgeIntersectionDirection" not in line) and ("Vector" not in line) and ("IntersectionDirection" not in line):
            line_split = line.strip().split(",")
            one_read.append(int(line_split[0])-BASE_VOLTAGE)
            one_timestamps.append(int(line_split[1]))
            two_read.append(int(line_split[2])-BASE_VOLTAGE)
            two_timestamps.append(int(line_split[3]))
            three_read.append(int(line_split[4])-BASE_VOLTAGE)
            three_timestamps.append(int(line_split[5]))
            four_read.append(int(line_split[6])-BASE_VOLTAGE)
            four_timestamps.append(int(line_split[7]))

    plt.plot(np.array(one_timestamps), np.array(one_read), label = "one")
    plt.plot(np.array(two_timestamps), np.array(two_read), label = "two")
    plt.plot(np.array(three_timestamps), np.array(three_read), label = "three")
    plt.plot(np.array(four_timestamps), np.array(four_read), label = "four")

    plt.legend()
    plt.xlabel("Time (ms)")
    plt.ylabel("Amplitude (V)")
    plt.show()

def sign(x):
    return math.copysign(1, x)

print(plot_sound_waves("2.csv"))
