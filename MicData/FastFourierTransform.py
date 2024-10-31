# https://www.diva-portal.org/smash/get/diva2:830430/FULLTEXT01.pdf

import matplotlib.pyplot as plt
import numpy as np
from scipy.fft import fft, fftfreq
import math

# time between polls is 44 microseconds
# 900 polls per sample

class ComplexNumber:
    def __init__(self, numpy_complex):
        self.real = numpy_complex.real
        self.imaginary = numpy_complex.imag
        self.magnitude = math.sqrt(self.real**2+self.imaginary**2)
        angle = (180/math.pi)*math.atan(self.imaginary/self.real)
        if (self.real < 0):
            angle += 180
        if (angle < 0):
            angle += 360
        self.phase = angle
    
    def __str__(self):
        return f"({self.real}, {self.imaginary}i) Magnitude: {self.magnitude} Phase: {self.phase}"


def get_time_differnce(file_name):
    file = open(file_name, "r")

    left_read = []
    left_timestamps = []
    right_read = []
    right_timestamps = []

    next(file)
    for line in file:
        line_split = line.strip().split(",")
        left_read.append(int(line_split[0])-330)
        left_timestamps.append(int(line_split[1]))
        right_read.append(int(line_split[2])-330)
        right_timestamps.append(int(line_split[3]))

    # maximum_sound = max(max(left_read), max(right_read))
    # left_read = [i/(maximum_sound**2) for i in left_read]
    # right_read = [i/(maximum_sound**2) for i in right_read]

    plt.plot(np.array(left_timestamps), np.array(left_read), label = "left", linestyle=":")
    plt.plot(np.array(right_timestamps), np.array(right_read), label = "right")
    plt.legend()
    plt.show()

    # Get the frequency components of the spectrum
    fourier_left = fft(left_read)
    fourier_right = fft(right_read)

    fourier_left_computed = [ComplexNumber(i) for i in fourier_left[:450]]
    fourier_right_computed = [ComplexNumber(i) for i in fourier_right[:450]]

    frequencies = fftfreq(900, 44*(10**-6))

    # print(dir(fourier_left[0]))
    # print(fourier_left[0])
    # print(fourier_left[0].real)
    # print(fourier_left[0].imag)
    # norm_amplitude = np.abs(fourier)/normalize
    # Plot the results
    # plt.plot([i.magnitude for i in fourier_left_computed], label = "left")
    # plt.plot([i.magnitude for i in fourier_right_computed], label = "right")
    # plt.show()

    def getTimeDifference(complexNumber1, complexNumber2, frequency):
        angle_difference = complexNumber1.phase-complexNumber2.phase
        return (1/frequency) * (angle_difference/360)

    combined_magnitudes = [fourier_left_computed[i].magnitude+fourier_right_computed[i].magnitude for i in range(1,450)]
    top_combined_magnitudes = sorted(range(len(combined_magnitudes)), key=lambda i: combined_magnitudes[i])[-15:]

    time_difference_sum = 0
    combined_magnitudes_sum = 0
    for i in top_combined_magnitudes:
        combined_magnitudes_sum += combined_magnitudes[i]
        time_difference_sum += combined_magnitudes[i]*getTimeDifference(fourier_left_computed[i], fourier_right_computed[i], frequencies[i])
    
    # return time_difference_sum/combined_magnitudes_sum

    # plt.plot([i.phase for i in fourier_left_computed], label = "left")
    # plt.plot([i.phase for i in fourier_right_computed], label = "right")
    # plt.show()


for i in range(1,2):
    print(get_time_differnce(f"Timestamp Corrected/0 Degrees/{i}.csv"))

# plt.plot([1000*getTimeDifference(fourier_left_computed[i], fourier_right_computed[i], frequencies[i]) for i in range(1,450)], label = "milliseconds")
# plt.plot(x, label = "magnitudes")
# plt.show()

# plt.plot([i.phase for i in fourier_left_computed], label = "left")
# plt.plot([i.phase for i in fourier_right_computed], label = "right")
# plt.show()

# # fourier = fft(left_read)
# # plt.plot(np.abs(fourier))
# # plt.show()
