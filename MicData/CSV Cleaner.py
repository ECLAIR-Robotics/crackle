filename = "90 degrees left"
# folder = "O Degrees"

i = 1
for j in range(2, 9):
    writefile = open(str(i)+".csv", "w+")
    for line in open(filename+str(j)+".csv", "r"):
        writefile.write(line.strip()[2:-5]+"\n")
    i += 1
