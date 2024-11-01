import matplotlib.pyplot as plt
import math

voltage = 330

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
        left_read.append(int(line_split[0])-voltage)
        left_timestamps.append(int(line_split[1]))
        right_read.append(int(line_split[2])-voltage)
        right_timestamps.append(int(line_split[3]))

    samples = len(left_read)
    time_step = (10**-6)*(left_timestamps[-1]-left_timestamps[0]+right_timestamps[-1]-right_timestamps[0])/(2*samples)

    cross_correlation = []
    left_read_padded = [0]*samples + left_read + [0]*samples
    for offset in range(-samples, samples):
        correlation = 0
        for i in range(0, samples):
            if 0 <= i+offset and i+offset < samples:
                correlation += right_read[i]*left_read[i+offset]
        cross_correlation.append(correlation)

    max_cross_correlation = cross_correlation.index(max(cross_correlation))

    # https://pages.hmc.edu/ruye/MachineLearning/lectures/ch3/node4.html
    # Parabolic interpolation

    a = max_cross_correlation-1
    b = max_cross_correlation
    c = max_cross_correlation+1
    fa = cross_correlation[a]
    fb = cross_correlation[b]
    fc = cross_correlation[c]
    # print(a, b, c, fa, fb, fc)
    interpolated_max_cross_correlation = b+0.5*((fa-fb)*(c-b)**2-(fc-fb)*(b-a)**2)/((fa-fb)*(c-b)+(fc-fb)*(b-a))

    # plt.figure()
    # plt.plot(list(range(-15,15)), cross_correlation[samples-15:samples+15], linestyle=":")
    # plt.plot(list(range(-15,15)), cross_correlation[samples-15:samples+15])
    # plt.axvline(x = max_cross_correlation-samples, color = 'b')
    # plt.axvline(x = interpolated_max_cross_correlation-samples, color = 'r')
    # plt.title("cross_correlation")
    # plt.show()
    
    return time_step*(interpolated_max_cross_correlation-samples)

d = 125/(2*(10**3)) # half mic seperation in m
c = 343 # speed of sound in m/s
def get_angle(t):
    # t is time difference in seconds
    if t == 0:
        return 0, "left/right"
    if 4*(d**2)<(c*t)**2:
        return 90, "left" if t<0 else "right"
    # degrees relative to 0 in the middle
    return 90-(180/math.pi)*math.atan(math.sqrt((4*(d**2)-(c*t)**2)/(c*t)**2)), "left" if t<0 else "right"

# cross_correlation(f"Timestamp Corrected/90 Degrees Right/1.csv")
for folder in ["90 Degrees Right", "45 Degrees Right", "0 Degrees", "45 Degrees Left", "90 Degrees Left"]:
    print(folder)
    for i in range(1,8):
        print(get_angle(cross_correlation(f"Timestamp Corrected/{folder}/{i}.csv")))
