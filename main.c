#include <stdio.h>
#include <time.h>
#include <stdlib.h>

struct node {
    int id;
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
        printf("id: %d | value: %f\n", nodes[nodeIt].id, nodes[nodeIt].value);
        for (int vertexIt = 0; vertexIt < n; vertexIt++)
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

int main( )
{
    int maxIterations = 1000;

    float p = 0.25f;
    int n = 800;
    int variant = 1;
    int rnv = 0;

    oneTimeInitStuff();

    for(int itCounter = 0; itCounter < maxIterations; itCounter++)
    {
        n = rand() % 1000 + 1;
        p = ((double)rand()) / ((double)RAND_MAX);

        struct node* nodes = malloc(sizeof(struct node) * n);

        printf("Initialize!\n");
        initializeNodes(nodes, n, variant, rnv);
        printf("Building Graph!\n");
        buildGraph(nodes, n, p);
        printf("Build Set!\n");
        writeLog(n, p, maxIndependentSet(nodes, n, variant, rnv), variant, rnv);
        //printGraph(nodes, n);

        freeStuff(nodes, n);
    }

    return 0;
}