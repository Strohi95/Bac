#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

typedef struct node Node;

struct node {
    int id;
    int neighbourCount;
    double value;
    Node* neighbours[];
};

int errorHandler(int errorCode)
{
    switch (errorCode)
    {
        case -1: printf("\n\nEMPTY TSV FILE!\n\n");
                 return 0;
        case -2: printf("\n\nMEMORY ISSUES!\n\n");
                 return 0;
        default: return 1;
    }
}

double numberGenerator(int rnv, int n)
{
    switch (rnv)
    {
        case 0: return ((double)rand() / (double)RAND_MAX);
        case 1: return (rand() % 100) + 1;
        case 2: return (rand() % n) + 1;
        default: return ((double)rand() / (double)RAND_MAX);
    }
}

int setCompleted(Node*** nodes, int n)
{
    for(int nodeIt = 0; nodeIt < n; nodeIt++)
        if((*nodes)[nodeIt]->value != 1111 && (*nodes)[nodeIt]->value != -10)
            return 0;

    return 1;
}

void setRValue(Node*** nodes, int n, int rnv)
{
    for(int nodeIt = 0; nodeIt < n; nodeIt++)
    {
        (*nodes)[nodeIt]->value = numberGenerator(rnv, n);
    }
}

int maxIndependentSet(Node*** nodes, int n, int variant, int rnv)
{
    int iterations = 0;

    while(!setCompleted(nodes, n))
    {
        if(variant == 1)
            continue; //setRValue(nodes, n, rnv);

        for (int nodeIt = 0; nodeIt < n; nodeIt++)
        {
            int localMax = 1, inSetN = 0;

            if ((*nodes)[nodeIt]->value == 1111 || (*nodes)[nodeIt]->value == -10)
                continue;
            //printf("nullzig\n\n");
            for (int nIt = 0; nIt < (*nodes)[nodeIt]->neighbourCount; nIt++)
            {
                //printf("\n\nDEBUG[NodeID: %d | NodeValue: %f | NeighborID: %d | NeighborValue: %f]\n\n",
                      // (*nodes)[nodeIt]->id ,(*nodes)[nodeIt]->value, (*nodes)[nodeIt]->neighbours[nIt]->id, (*nodes)[nodeIt]->neighbours[nIt]->value);
                if ((*nodes)[nodeIt]->neighbours[nIt] == NULL)
                    continue;
                //printf("einszig\n\n");
                if ((*nodes)[nodeIt]->value < (*nodes)[nodeIt]->neighbours[nIt]->value)
                    localMax = 0;
                //printf("schwierig\n\n");
                if ((*nodes)[nodeIt]->neighbours[nIt]->value == 1111)
                    inSetN = 1;
            }
            //printf("zweizig\n\n");
            if (localMax)
                (*nodes)[nodeIt]->value = 1111;
            if (inSetN)
                (*nodes)[nodeIt]->value = -10;
        }
        iterations++;
    }

    return iterations;
}

void writeLog(int n, float p, int iterations, int variant, int rnv)
{
    if(p == 0.f)
    {
        FILE *fptr = fopen("logs/TSVlogFile.txt", "a+");
        fprintf(fptr, "Variant: %d\tNodes: %d\tIterations: %d\n", variant, n, iterations);
        fclose(fptr);
    }
    else
    {
        FILE *fptr = fopen("logs/logFile01.txt", "a+");
        fprintf(fptr, "Variant: %d\tRandomNumber: %d\tNodes: %d\tIterations: %d\n", variant, rnv, n, iterations);
        fclose(fptr);
    }
}

void logGraph(Node **nodes, int n)
{
    FILE *fptr = fopen("logs/tsvGraph.txt", "a+");

    for(int nodeIt = 0; nodeIt < n; nodeIt++)
    {
        fprintf(fptr, "id: %d | nCount: %d | value: %f\n", nodes[nodeIt]->id, nodes[nodeIt]->neighbourCount, nodes[nodeIt]->value);
        for (int vertexIt = 0; vertexIt < nodes[nodeIt]->neighbourCount; vertexIt++)
        {
            fprintf(fptr,"\tvertex %d to node %d\n", vertexIt, nodes[nodeIt]->neighbours[vertexIt]->id);
        }
    }
    fclose(fptr);
}

void printGraph(Node **nodes, int n)
{
    printf("\nPrinting current Graph with %d nodes!\n", n);

    for(int nodeIt = 0; nodeIt < n; nodeIt++)
    {
        printf("id: %d | nCount: %d | value: %f\n", nodes[nodeIt]->id, nodes[nodeIt]->neighbourCount, nodes[nodeIt]->value);
        for (int vertexIt = 0; vertexIt < nodes[nodeIt]->neighbourCount; vertexIt++)
        {
            printf("\tvertex %d to node %d\n", vertexIt, nodes[nodeIt]->neighbours[vertexIt]->id);
        }
    }

    printf("\n\n\n");
}

void freeStuff(Node*** nodes, int n)
{
    /*for(int nodeIt = 0; nodeIt < n; nodeIt++)
    {
        free(nodes[nodeIt]);
    }*/

    free(*nodes);  // This should be outside the loop
}

int checkIfNodeExist(Node** nodes, int nodeCount, int nodeID)
{
    if(nodeCount == 0)
        return -1;

    for(int nodeIt = 0; nodeIt < nodeCount; nodeIt++)
    {
        if(nodes[nodeIt]->id == nodeID)
        {
            return nodeIt;
        }
    }

    return -1;
}

int addNode(Node*** nodes, int nodeCount, int nodeID)
{
    //printf("nC: %d\n", nodeCount);

    if(nodeCount > 1)
    {
        *nodes = realloc(*nodes, sizeof(Node*) * (nodeCount + 1));
    }

    (*nodes)[nodeCount-1] = malloc(sizeof(Node) + sizeof(Node*));

    (*nodes)[nodeCount-1]->id = nodeID;
    (*nodes)[nodeCount-1]->value = numberGenerator(0, 0);
    (*nodes)[nodeCount-1]->neighbourCount = 0;

    //printf("ID STFUF; %d | NODEID: %d\n", (*nodes)[nodeCount-1]->id, nodeID);

    return nodeCount-1;
}

int addNeighbors(Node*** nodes, int* nodeCount, int nodePos0, int nodePos1)
{
    Node* tempNode1 = (*nodes)[nodePos1];
    Node* tempNode0 = (*nodes)[nodePos0];

    //printf("n from %d to %d\n\n----------------------------------\n\n", (*nodes)[nodePos0]->id, (*nodes)[nodePos1]->id);

    Node* updatedNodePos0 = realloc((*nodes)[nodePos0], sizeof(Node) + sizeof(Node*) * ((*nodes)[nodePos0]->neighbourCount + 1));
    Node* updatedNodePos1 = realloc((*nodes)[nodePos1], sizeof(Node) + sizeof(Node*) * ((*nodes)[nodePos1]->neighbourCount + 1));

    if (updatedNodePos0 == NULL || updatedNodePos1 == NULL)
        return -1;

    (*nodes)[nodePos0] = updatedNodePos0;
    (*nodes)[nodePos1] = updatedNodePos1;

    /*printf("from nodePos1: %d\n", tempNode->id);
    printf("from nodePos1: %d\n", updatedNodePos1->id);
    printf("from nodePos1: %d\n", (*nodes)[nodePos1]->id);*/
    for(int nIt = 0; nIt < updatedNodePos0->neighbourCount; nIt++)
    {
        for(int nnIt = 0; nnIt < updatedNodePos0->neighbours[nIt]->neighbourCount; nnIt++)
        {
            if(updatedNodePos0->neighbours[nIt]->neighbours[nnIt] == tempNode0)
                updatedNodePos0->neighbours[nIt]->neighbours[nnIt] = updatedNodePos0;
        }
        //printf("\nto n: %d\n", updatedNodePos0->neighbours[nIt]->id);
    }


    for(int nIt = 0; nIt < updatedNodePos1->neighbourCount; nIt++)
    {
        for(int nnIt = 0; nnIt < updatedNodePos1->neighbours[nIt]->neighbourCount; nnIt++)
        {
            if(updatedNodePos1->neighbours[nIt]->neighbours[nnIt] == tempNode1)
                updatedNodePos1->neighbours[nIt]->neighbours[nnIt] = updatedNodePos1;
        }
        //printf("\nto n: %d\n", updatedNodePos1->neighbours[nIt]->id);
    }

    (*nodes)[nodePos0]->neighbours[(*nodes)[nodePos0]->neighbourCount] = (*nodes)[nodePos1];
    (*nodes)[nodePos1]->neighbours[(*nodes)[nodePos1]->neighbourCount] = (*nodes)[nodePos0];

    (*nodes)[nodePos0]->neighbourCount++;
    (*nodes)[nodePos1]->neighbourCount++;

    return 0;
}

int NodeInputHandler(Node*** nodes, int* nodeCount, int nodeID0, int nodeID1)
{
    int node0Pos = checkIfNodeExist(*nodes, *nodeCount, nodeID0);
    int node1Pos = checkIfNodeExist(*nodes, *nodeCount, nodeID1);

    if(node0Pos == -1) {
        node0Pos = addNode(nodes, ++(*nodeCount), nodeID0);
    }
    //printGraph(*nodes, *nodeCount);
    if(node1Pos == -1) {
        node1Pos = addNode(nodes, ++(*nodeCount), nodeID1);
    }

    addNeighbors(nodes, nodeCount, node0Pos, node1Pos);

    //printGraph(*nodes, *nodeCount);
    return *nodeCount;
}

int createGraph()
{
    size_t len = 0;
    char* line = NULL;
    int lineCount = 0;
    const char* s = "\t";
    int numIterations = 100;

    int nodeID0, nodeID1;

    char *graphSelection = "graphs/MAWI_Datasets_Graph_1.tsv";

    FILE *tsvFile = fopen ( graphSelection, "r+" );

    Node** nodes = malloc(sizeof(Node*));
    int nodeCount = 0;

    while (getline(&line, &len, tsvFile) != -1 && nodeCount < 1000000)
    {
        nodeID0 = atoi(strtok(line, s));
        nodeID1 = atoi(strtok(NULL, s));

        NodeInputHandler(&nodes, &nodeCount, nodeID0, nodeID1);

        //printf("%d", lineCount);+

        lineCount++;
    }

    //logGraph(nodes, nodeCount);

    for(int i = 0; i < numIterations; i++)
    {
        writeLog(nodeCount, 0.f, maxIndependentSet(&nodes, nodeCount, 2, 0), 2, 0);
        setRValue(&nodes, nodeCount, 0);
    }

    freeStuff(&nodes, nodeCount);
    return 0;
}

int main()
{
    return createGraph();
}
