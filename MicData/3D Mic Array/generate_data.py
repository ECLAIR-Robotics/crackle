# everything in m and s
# 1 mm = 0.001 m

import math
import numpy as np

c = 343 # m/s

mic_locations = [
    np.array([0.075, 0.075, 0]), # 0
    np.array([0.075, -0.075, 0]), # 1
    np.array([-0.075, 0.075, 0]), # 2
    np.array([-0.075, -0.075, 0]) # 3
]

mic_pairings = [
    [0, 1],
    [0, 2],
    [0, 3],
    [1, 2],
    [1, 3],
    [2, 3]
]

class TimeDifference:
  def __init__(self, mic_locations, delta_t):
    self.mic_locations = mic_locations
    self.delta_t = delta_t

def get_time_differences(sound_location):
    mic_times = []
    for mic in mic_locations:
        distance = math.sqrt(
            (sound_location[0]-mic[0]) ** 2 +
            (sound_location[1]-mic[1]) ** 2 +
            (sound_location[2]-mic[2]) ** 2
        )
        mic_times.append(distance/c)
    mic_time_differences = []
    for pairing in mic_pairings:
        mic_time_differences.append(TimeDifference([mic_locations[pairing[0]], mic_locations[pairing[1]]], mic_times[pairing[0]] - mic_times[pairing[1]]))
    return mic_time_differences