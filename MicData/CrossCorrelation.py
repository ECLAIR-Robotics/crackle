import matplotlib.pyplot as plt
import math

# time between polls is 44 microseconds
# 900 polls per sample

def cross_correlation(file_name):
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

    samples = len(left_read)

    # plt.figure()
    # plt.plot(left_read[:20], label = "left", linestyle=":")
    # plt.plot(right_read[:20], label = "right")
    # plt.title("data")

    cross_correlation = []
    left_read_padded = [0]*samples + left_read + [0]*samples
    for i in range(0, 2*samples+1):
        right_read_padded = [0]*i + right_read + [0]*(2*samples-i)
        cross_correlation.append(sum([left_read_padded[i]*right_read_padded[i] for i in range(samples*3)]))

    # plt.figure()
    # plt.plot(list(range(-samples,samples+1)), cross_correlation, linestyle=":")
    # plt.title("cross_correlation")
    # plt.show()
    
    max_cross_correlation = cross_correlation.index(max(cross_correlation))

    return cross_correlation.index(max(cross_correlation))-900

d = 125/2 # half mic seperation in mm
c = 343 # speed of sound in m/s
def get_angle(t):
    # t is time difference in milliseconds
    if t == 0:
        return 0, "left/right"
    if 4*(d**2)<(c*t)**2:
        return 90, "left" if t<0 else "right"
    return 90-(180/math.pi)*math.atan(math.sqrt((4*(d**2)-(c*t)**2)/(c*t)**2)), "left" if t<0 else "right"

for folder in ["90 Degrees Right", "45 Degrees Right", "0 Degrees", "45 Degrees Left", "90 Degrees Left"]:
    print(folder)
    for i in range(1,8):
        print(cross_correlation(f"Timestamp Corrected/{folder}/{i}.csv"), get_angle(0.044*cross_correlation(f"Timestamp Corrected/{folder}/{i}.csv")))
