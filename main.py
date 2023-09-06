import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d

values = [] * 5
x = []
y = []
z = []


def readFile():
    logFile = open("../logs/logFile.txt")
    lines = logFile.readlines()

    for line in lines:
        valueEntry = [0, 0, 0, 0, 0]

        splitLine = line.split("\t")
        valueEntry[0] = splitLine[0].split(" ")[1]
        valueEntry[1] = splitLine[1].split(" ")[1]
        valueEntry[2] = splitLine[2].split(" ")[1]
        x.append(valueEntry[2])
        valueEntry[3] = splitLine[3].split(" ")[1]
        y.append(valueEntry[3])
        valueEntry[4] = splitLine[4].split(" ")[1][:-1]
        z.append(valueEntry[4])

        values.append(valueEntry)


def clearData():
    return

def drawPlot(x, y, z):
    fig = plt.figure()

    # syntax for 3-D projection
    ax = plt.axes(projection='3d')

    # defining axes
    x = res = [eval(i) for i in x]
    y = res = [eval(i) for i in y]
    z = res = [eval(i) for i in z]


    ax.scatter(x, y, z)

    # syntax for plotting
    ax.set_title('teststuff')
    plt.show()

def main():
    readFile()
    drawPlot(x, y, z)


if __name__ == '__main__':
    main()
