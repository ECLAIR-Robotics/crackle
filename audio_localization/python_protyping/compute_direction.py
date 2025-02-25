import math
import numpy as np

from generate_data import get_time_differences

c = 343 # m/s

def rotaion_matrix_around_axis(axis, theta):
    # https://en.wikipedia.org/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
    return np.matrix([
        [
            (axis[0]**2)*(1-math.cos(theta))+math.cos(theta), 
            axis[0]*axis[1]*(1-math.cos(theta))-axis[2]*math.sin(theta), 
            axis[0]*axis[2]*(1-math.cos(theta))+axis[1]*math.sin(theta)
        ],
        [
            axis[0]*axis[1]*(1-math.cos(theta))+axis[2]*math.sin(theta),
            (axis[1]**2)*(1-math.cos(theta))+math.cos(theta),
            axis[1]*axis[2]*(1-math.cos(theta))-axis[0]*math.sin(theta)
        ],
        [
            axis[0]*axis[2]*(1-math.cos(theta))-axis[1]*math.sin(theta),
            axis[1]*axis[2]*(1-math.cos(theta))+axis[0]*math.sin(theta),
            (axis[2]**2)*(1-math.cos(theta))+math.cos(theta)
        ]
    ])

def normalize(v):
    return v/np.linalg.norm(v)
    
def get_slope(time_difference, seperation):
    return math.sqrt((seperation**2 - (c * time_difference)**2))/(c * time_difference)

def get_cone_intersection_direction(n1, s1, n2, s2):

    n1 = normalize(n1)
    n2 = normalize(n2)

    alpha = math.atan(s1)
    # print("alpha")
    # print(alpha)
    beta = math.atan(s2)
    # print("beta")
    # print(beta)

    theta = math.acos(np.dot(n1, n2))
    # print("theta")
    # print(theta)

    n2_cross_n1 = np.cross(n2, n1)
    # print("n2_cross_n1")
    # print(n2_cross_n1)

    # if alpha + beta < theta:
    #     angle_to_cones_midpoint = beta + (theta - alpha - beta)/2

    #     return np.array(np.matmul(rotaion_matrix_around_axis(normalize(n2_cross_n1), angle_to_cones_midpoint), n2).tolist()[0])

    z = math.cos(alpha)
    # print("z")
    # print(z)
    w = np.array([0, math.sin(theta-beta), math.cos(theta-beta)])
    # print("w")
    # print(w)

    beta_z1 = math.cos(theta-beta)
    beta_z2 = math.cos(theta+beta)

    if ((beta_z1 > z) and (beta_z2 > z)) or ((beta_z1 < z) and (beta_z2 < z)):
        angle_differences = [
            abs((theta-beta) - alpha),
            abs((theta+beta) - alpha),
            abs((theta-beta) + alpha),
            abs((theta+beta) + alpha),
        ]

        angle_off_n2 = None
        min_angle_difference = angle_differences.index(min(angle_differences))

        if min_angle_difference == 0:
            angle_off_n2 = beta + ((theta-beta)-alpha)/2
        elif min_angle_difference == 1:
            angle_off_n2 = -beta + ((theta+beta)-alpha)/2
        elif min_angle_difference == 2:
            angle_off_n2 = beta + ((theta-beta)+alpha)/2
        elif min_angle_difference == 3:
            angle_off_n2 = -beta + ((theta+beta)+alpha)/2

        print((180/math.pi)*angle_off_n2)

        return np.array(np.matmul(rotaion_matrix_around_axis(normalize(n2_cross_n1), -angle_off_n2), n2).tolist()[0])

    n = np.array([0, math.sin(theta), math.cos(theta)]) # n2 rotated away from n1 on z-axis
    # print("n")
    # print(n)

    angle_of_intersection = math.acos((z-w[1]*n[1]*n[2]-(n[2]**2)*w[2])/(w[2]-w[1]*n[1]*n[2]-(n[2]**2)*w[2]))
    # print("angle_of_intersection")
    # print(angle_of_intersection)

    v = np.array(np.matmul(rotaion_matrix_around_axis(normalize(n2_cross_n1), beta), n2).tolist()[0])
    # print("rotaion_matrix_around_axis(normalize(n2_cross_n1), beta)")
    # print(rotaion_matrix_around_axis(normalize(n2_cross_n1), beta))

    # print("v")
    # print(v)

    intersection_direction1 = np.array(np.matmul(rotaion_matrix_around_axis(n2, angle_of_intersection),v).tolist()[0])
    # print("intersection_direction1")
    # print(intersection_direction1)
    intersection_direction2 = np.array(np.matmul(rotaion_matrix_around_axis(n2, -angle_of_intersection),v).tolist()[0])
    # print("intersection_direction1")
    # print(intersection_direction1)
        

    if intersection_direction1[2] > 0: # we assume our sound is in front of mic array. This assumption will not hold in the future when we fix the 3D issues
        return intersection_direction1
    else:
        return intersection_direction2

class Cone:
  def __init__(self, normal, slope):
    self.normal = normal
    self.slope = slope

def get_sound_direction(time_differences):

    cones = [] # [normal, slope]
    for time_difference in time_differences:
        normal = normalize(np.array((time_difference.mic_locations[1] - time_difference.mic_locations[0]) * math.copysign(1, time_difference.delta_t)))
        slope = get_slope(abs(time_difference.delta_t), np.linalg.norm(time_difference.mic_locations[0] - time_difference.mic_locations[1]))
        cones.append(Cone(normal, slope))

    cones_pairing_sum = 0
    cone_intersection_direction_sum = np.array([0,0,0])
    for i in range(0, len(cones)-1):
        for j in range(i+1, len(cones)):
            # if i == 0 and j == 5:
            # if not np.array_equal(cones[i].normal, cones[j].normal) and i*j != 10 and i*j != 3:
            if not np.array_equal(cones[i].normal, cones[j].normal):
                print(i, j)
                cones_pairing_sum += 1
                cone_intersection_direction_sum = cone_intersection_direction_sum + get_cone_intersection_direction(cones[i].normal, cones[i].slope, cones[j].normal, cones[j].slope)

    return cone_intersection_direction_sum/cones_pairing_sum

sound_origin = [-16,10,7] # z must be positive currently

time_differences = get_time_differences(sound_origin)

# for i in time_differences:
#     print(i.delta_t)
# print(time_differences)

print(get_cone_intersection_direction(np.array([0, 1, 0]), 0.67, np.array([0.71, 0.71, 0]), 276.68))

# print("input", sound_origin)
# print("output", get_sound_direction(time_differences))
# print("desired", normalize(np.array(sound_origin)))