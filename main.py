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
    fig = plt.figure()

    figure, axis = plt.subplots(1, 2, sharey=True, figsize = (20, 10))

    nodes = [eval(i) for i in Nodes_]
    p_vertex = [eval(i) for i in P_Vertex_]
    iterations = [eval(i) for i in Iterations_]

    axis[0].scatter(nodes, iterations, s=10, c="green", label="Nodes")
    axis[0].legend(loc="upper left")
    axis[0].title.set_text("Iterations per Num(Nodes)")

    axis[1].scatter(p_vertex, iterations, s=10, c="blue", label="P(Vertex)")
    axis[1].legend(loc="upper left")
    axis[1].title.set_text("Iterations per P(Vertex)")

    plt.show()


def main():
    readFile()
    drawPlot(Nodes_, P_Vertex_, Iterations_)


if __name__ == '__main__':
    main()
