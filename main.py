from typing import List, Any

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits import mplot3d

values = [] * 5
Nodes_: List[int] = []
P_Vertex_: List[float] = []
Iterations_: List[int] = []


def readFile():
    logFile = open("logs/logFile.txt")
    lines = logFile.readlines()

    for line in lines:
        valueEntry = [0, 0, 0, 0, 0]

        splitLine = line.split("\t")
        valueEntry[0] = splitLine[0].split(" ")[1]
        valueEntry[1] = splitLine[1].split(" ")[1]
        valueEntry[2] = splitLine[2].split(" ")[1]
        Nodes_.append(valueEntry[2])
        valueEntry[3] = splitLine[3].split(" ")[1]
        P_Vertex_.append(valueEntry[3])
        valueEntry[4] = splitLine[4].split(" ")[1][:-1]
        Iterations_.append(valueEntry[4])

        values.append(valueEntry)


def clearData():
    return


def drawPlot(x, p_vertex, iterations):
    fig = plt.figure(figsize=(20, 10))

    axis = fig.add_subplot(2, 2, 1)
    nodes = [eval(i) for i in Nodes_]
    iterations = [eval(i) for i in Iterations_]
    axis.scatter(nodes, iterations, s=2, c="green", label="Nodes")
    axis.legend(loc="upper left")
    axis.set_title("Iterations per Num(Nodes)")

    axis = fig.add_subplot(2, 2, 3)
    p_vertex = [eval(i) for i in P_Vertex_]
    axis.scatter(p_vertex, iterations, s=2, c="blue", label="P(Vertex)")
    axis.legend(loc="upper left")
    axis.set_title("Iterations per P(Vertex)")

    axis = fig.add_subplot(1, 2, 2, projection='3d')
    axis.plot_trisurf(nodes, p_vertex, iterations)
    axis.view_init(elev=10., azim=20)
    axis.set_title("Iterations per Nodes and P(Vertex)")

    plt.show()


def main():
    readFile()
    drawPlot(Nodes_, P_Vertex_, Iterations_)


if __name__ == '__main__':
    main()
