int_length = 32768
last_timestep_left = None
last_timestep_right = None

folder = "Raw Data/90 Degrees Right"

for i in range(1,8):
    last_timestep_left = None
    last_timestep_right = None
    old_file = open(folder+"/"+str(i)+".csv", "r")
    new_file = open(str(i)+".csv", "w")
    new_file.write(old_file.readline())
    for line in old_file:
        line_split = [int(i) for i in line.strip().split(",")]
        if (line_split[1] < 0):
            line_split[1] += 2*int_length
        if (line_split[3] < 0):
            line_split[3] += 2*int_length
        if last_timestep_left != None and line_split[1] < last_timestep_left:
            while line_split[1] < last_timestep_left:
                line_split[1] += int_length
        if last_timestep_right != None and line_split[3] < last_timestep_right:
            while line_split[3] < last_timestep_right:
                line_split[3] += int_length
        new_file.write(",".join([str(i) for i in line_split])+"\n")
        last_timestep_left = line_split[1]
        last_timestep_right = line_split[3]

