import matplotlib.pyplot as plt
import numpy as np
import os

servo_readouts_folder = "servo_readouts"

for file in os.listdir(servo_readouts_folder):
    data = [int(x) for x in open(servo_readouts_folder+"/"+file, 'r').readlines()]
    plt.plot(list(range(len(data))), data, label = file)
    
plt.legend()
plt.show()