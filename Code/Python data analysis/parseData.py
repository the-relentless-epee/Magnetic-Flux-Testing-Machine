import numpy as np
import matplotlib.pyplot as plt

file_path = "data/datalog1.txt"
sampleCount = 1000
stepSize = 0.1

arrays =[]

with open(file_path, "r") as file:
    data = np.genfromtxt(file_path, delimiter=",")

cycleCount = int(len(data)/sampleCount)
for i in range(cycleCount):
    tempArr = data[i*sampleCount:(i+1)*sampleCount, :]
    val = np.average(tempArr[:, 1])
    arr = [i*stepSize, val]


    globals()[f"arr{i}"] = arr  #creates n number of arrays based on how many cycles there are.
    arrays.append(arr) #creates array of data arrays.

#print(arrays)
# plotArray = np.array(arrays)
plotArray = np.array(arrays)[:,:]
plt.scatter(plotArray[:,0], plotArray[:,1])
plt.title("avg plot")
plt.show()