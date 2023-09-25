#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

struct node {
    int id;
    int neighbourCount;
    struct node** neighbours;
    double value;
};

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

int initializeNodes(struct node* nodes, int n, int variant, int rnv)
{
    for(int nodeIt = 0; nodeIt < n; nodeIt++)
    {
        nodes[nodeIt].id = nodeIt;
        nodes[nodeIt].neighbours = malloc(sizeof(struct node*) * n);
        if(variant == 0)
            nodes[nodeIt].value = numberGenerator(rnv, n);
        else if(variant == 1)
            nodes[nodeIt].value = 0;

        for(int vertexIt = 0; vertexIt < n; vertexIt++)
            nodes[nodeIt].neighbours[vertexIt] = NULL;
    }

    return 0;
}

int buildGraph(struct node* nodes, int n, float p)
{
    for(int nodeIt = 0; nodeIt < n; nodeIt++)
    {
        for(int vertexIt = 0; vertexIt < n; vertexIt++)
        {
            if(nodeIt == vertexIt)
                continue;

            float ran = numberGenerator(0, n);

            if(ran <= p)
            {
                nodes[nodeIt].neighbours[vertexIt] = &nodes[vertexIt];
                nodes[vertexIt].neighbours[nodeIt] = &nodes[nodeIt];
            }
        }
    }

    return 0;
}

int setCompleted(struct node* nodes, int n)
{
    for(int nodeIt = 0; nodeIt < n; nodeIt++)
        if(nodes[nodeIt].value != 1111 && nodes[nodeIt].value != -10)
            return 0;

    return 1;
}

void setRValue(struct node* nodes, int n, int rnv)
{
    for(int nodeIt = 0; nodeIt < n; nodeIt++)
    {
        if(nodes[nodeIt].value != 1111 && nodes[nodeIt].value != -10)
            nodes[nodeIt].value = numberGenerator(rnv, n);
    }
}

int maxIndependentSet(struct node* nodes, int n, int variant, int rnv)
{
    int iterations = 0;

    while(!setCompleted(nodes, n))
    {
        if(variant == 1)
            setRValue(nodes, n, rnv);

        for (int nodeIt = 0; nodeIt < n; nodeIt++)
        {
            int localMax = 1, inSetN = 0;

            if (nodes[nodeIt].value == 1111 || nodes[nodeIt].value == -10)
                continue;

            for (int nIt = 0; nIt < n; nIt++)
            {
                if (nodes[nodeIt].neighbours[nIt] == NULL)
                    continue;

                if (nodes[nodeIt].value < nodes[nodeIt].neighbours[nIt]->value)
                    localMax = 0;
                if (nodes[nodeIt].neighbours[nIt]->value == 1111)
                    inSetN = 1;
            }

            if (localMax)
                nodes[nodeIt].value = 1111;
            if (inSetN)
                nodes[nodeIt].value = -10;
        }

        iterations++;
    }

    return iterations;
}

void writeLog(int n, float p, int iterations, int variant, int rnv)
{
    FILE *fptr = fopen("logs/logFile.txt", "a+");

    fprintf(fptr, "Variant: %d\tRandomNumber: %d\tNodes: %d\tP(Vertex): %.2f\tIterations: %d\n", variant, rnv, n, p, iterations);

    fclose(fptr);
}

void printGraph(struct node* nodes, int n)
{
    printf("\nPrinting current Graph!\n");

    for(int nodeIt = 0; nodeIt < n; nodeIt++)
    {
        printf("id: %d | ncount: %d\n", nodes[nodeIt].id, nodes[nodeIt].neighbourCount);
        for (int vertexIt = 0; vertexIt < nodes[nodeIt].neighbourCount; vertexIt++)
        {
            if(nodes[nodeIt].neighbours[vertexIt] != NULL)
                printf("\tvertex %d to node %d\n", vertexIt, nodes[nodeIt].neighbours[vertexIt]->id);
        }
    }

    printf("\n\n\n");
}

void oneTimeInitStuff()
{
    srand(time(NULL));
}

void freeStuff(struct node* nodes, int n)
{
    for(int nodeIt = 0; nodeIt < n; nodeIt++)
    {
        free(nodes[nodeIt].neighbours);
    }

    free(nodes);
}

void createNewGraph()
{
    int maxIterations = 1000;

    float p = 0.25f;
    int n = 800;
    int variant = 1;
    int rnv = 0;

    oneTimeInitStuff();

    for (int itCounter = 0; itCounter < maxIterations; itCounter++)
    {
        n = rand() % 1000 + 1;
        p = ((double) rand()) / ((double) RAND_MAX);

        struct node *nodes = malloc(sizeof(struct node) * n);

        printf("Initialize!\n");
        initializeNodes(nodes, n, variant, rnv);
        printf("Building Graph!\n");
        buildGraph(nodes, n, p);
        printf("Build Set!\n");
        writeLog(n, p, maxIndependentSet(nodes, n, variant, rnv), variant, rnv);
        //printGraph(nodes, n);

        freeStuff(nodes, n);
    }
}

int buildTSVGraph(char* graph)
{
    size_t len = 0;
    ssize_t read;
    char* line = NULL;
    int nodeCount = 2;
    const char* s = "\t";
    void* allocReturn;

    struct node *nodes = malloc(sizeof(struct node) * 2);

    FILE *tsvFile = fopen ( graph, "r+" );

    //Manually add first two Nodes to array
    if((read = getline(&line, &len, tsvFile)) != -1)
    {

        nodes[0].id = atoi(strtok(line, s));
        nodes[0].neighbours = malloc(sizeof(struct node*));
        nodes[0].neighbourCount += 1;
        nodes[1].id = atoi(strtok(NULL, s));
        nodes[1].neighbours = malloc(sizeof(struct node*));
        nodes[1].neighbourCount += 1;

        nodes[0].neighbours[0] = &nodes[1];
        nodes[1].neighbours[0] = &nodes[0];
    }
    else
        return -1;

    while ((read = getline(&line, &len, tsvFile)) != -1 && nodeCount < 100)
    {
        int existONode = 0;
        int existTNode = 0;
        int originID = atoi(strtok(line, s));
        int targetID = atoi(strtok(NULL, s));
        int originPos = 0;
        int targetPos = 0;

        int itCounter = 0;

        do
        {
            //printf("nodes[itCounter].id: %d, originID: %d, targetID: %d\n", nodes[itCounter].id, originID, targetID);

            if(nodes[itCounter].id == originID)
            {
               // printf("EXIT NODE ID\n");
                originPos = itCounter;
                existONode = 1;
            }
            if(nodes[itCounter].id == targetID)
            {
                // printf("EXIST TARGET ID\n");
                targetPos = itCounter;
                existTNode = 1;
            }

            itCounter++;
        }
        while (itCounter < nodeCount);

        //printGraph(nodes, nodeCount);
        //printf("\n\n\n\n");

        if(!existONode)
        {
            //printf("exist nooooooot\n");

            allocReturn = realloc(nodes, sizeof(struct node) * (++nodeCount));
            //printf("NodeCount: %d\n", nodeCount);
            if(allocReturn == NULL)
                return -2;
            nodes = allocReturn;
            nodes[nodeCount-1].id = originID;
            nodes[nodeCount-1].neighbourCount = 1;
            nodes[nodeCount-1].neighbours = malloc(sizeof(struct node*));
            originPos = nodeCount-1;
        }
        else
        {
            //printf("exist\n");
            nodes[originPos].neighbourCount += 1;
            allocReturn = realloc(nodes[originPos].neighbours, sizeof(struct node*) * (nodes[originPos].neighbourCount));
            if(allocReturn == NULL)
                return -2;
            nodes[originPos].neighbours = allocReturn;
        }

        if(!existTNode)
        {
            allocReturn = realloc(nodes, sizeof(struct node) * (++nodeCount));
            if(allocReturn == NULL)
                return -2;
            nodes = allocReturn;
            nodes[nodeCount-1].id = targetID;
            nodes[nodeCount-1].neighbourCount = 1;
            nodes[nodeCount-1].neighbours = malloc(sizeof(struct node*));
            targetPos = nodeCount-1;
        }
        else
        {
            nodes[targetPos].neighbourCount += 1;
            allocReturn = realloc(nodes[targetPos].neighbours, sizeof(struct node*) * (nodes[targetPos].neighbourCount));
            if(allocReturn == NULL)
                return -2;
            nodes[targetPos].neighbours = allocReturn;
        }

        nodes[originPos].neighbours[nodes[originPos].neighbourCount-1] = &nodes[targetPos];
        nodes[targetPos].neighbours[nodes[targetPos].neighbourCount-1] = &nodes[originPos];
    }

    printGraph(nodes, nodeCount);

    return 0;
}

int createTSVGraph()
{
    char *graphSelection = "graphs/MAWI_Datasets_Graph_1.tsv";

    buildTSVGraph(graphSelection);

    return 0;
}

int runAlgorithm(int graphType)
{
    switch (graphType)
    {
        case 1: createNewGraph();
        case 2: return createTSVGraph();
        default: createNewGraph();
    }

    return 0;
}

int main( )
{
    return runAlgorithm(2);;
}