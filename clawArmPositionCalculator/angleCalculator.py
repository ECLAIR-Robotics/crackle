from enum import Enum
from math import tan, radians

# Equations of each arm
arm_angles = []


# returns how many degs apart each arm is
def angle_delta():
    return 360 / len(ClawIdentity)


# uses num of arms (assuming that they are equidistant) to generate their equation constants
def make_equations():
    num_arms = len(ClawIdentity)
    for i in range(num_arms):
        arm_angles.append(angle_delta() * i)


# identity of the claw
class ClawIdentity(Enum):
    BASE_ARM = 0  # the one across from realsense
    CLOCKWISE = 1  # the one clockwise of the realsense
    COUNTERCLOCKWISE = 2  # the one ccw in relation to realsense


# returns the y val of the
def getHighestYPoints(points):
    current = points[0]
    next = points[1]

    i = 0

    # go till we are going right
    while current[0] > next[0]:
        i += 1
        current = points[i]
        next = points[i + 1]

    # while we are going up
    while current[1] < next[1]:
        i += 1
        current = points[i]
        next = points[i + 1]

    # have reached the highest point
    return current[1]


# represents each claw arm
class ClawArm:

    # angle each arm is from eachother
    offset_angle = angle_delta()

    # init
    def __init__(self, x_cord, y_cord, z_cord, degs, id):
        self.x = x_cord
        self.y = y_cord
        self.z = z_cord
        self.degs = degs
        self.id = id  # used as an the clawIdentity enum

    # TODO: populate with actual code to zero when we make a zeroing sequence
    def zero(self):
        self.x = 0
        self.y = 0
        self.z = 0
        self.rads = 0

    # finds the closest point on the perimeter to the line for this claw
    def getClosestPoint(self, points):
        # stupid special cases where the slope will be undefined
        angle = arm_angles[self.id]
        if angle % 180 == 0:
            # if highest, look for highest y

            # if lowest look for lowest y
            ...
        else:
            ...
        # if its the baseArm
