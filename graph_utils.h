//
// Created by stach on 08.05.2020.
//

#ifndef LOUVAIN_GRAPH_UTILS_H
#define LOUVAIN_GRAPH_UTILS_H

#include "dataio.h"

#define EDGES_IDX(graph, vertice) ((vertice) >=0 ? (graph)->verticeLastEdgeExclusive[(vertice)] : 0)

typedef struct{
    int from;
    int to;
    float weight;
} Edge;

typedef  struct{
    int size;
    int numEdges;
    Edge* edges;
    int* verticeLastEdgeExclusive;
} Graph;

void sortEdges(Graph* g);

int compareEdges(const void* a, const void* b);

Graph* initGraph(MData* data);

void destroyGraph(Graph* g);

void printGraph(Graph* g);

void printEdges(Graph* g);

float hasEdge(Graph *g, int from, int to);

void addEdge(Graph* g, int index, int from, int to, float weight);

#endif //LOUVAIN_GRAPH_UTILS_H
