//
// Created by stach on 08.05.2020.
//

#include "graph_utils.h"
#include "dataio.h"
//#include "dataio.c"

Graph* initGraph(MData* data){
    if(data->format.format == ARRAY || data->format.format == SKEW){
        THROW("arrayand skew  matrix not supported", 16);
    }
    int selfLoops = 0;
    for(int i = 0; i < data->size; i++){
        if(data->from[i] == data->to[i]) {
            selfLoops++;
        }
    }
    int edges = (data->size - selfLoops);
    if(data->format.symmetry == SYMMETRIC){
        edges *= 2;
    }

    Graph* g = malloc(sizeof(Graph));
    g->verticeLastEdgeExclusive = (int*)malloc(sizeof(int) * data->rows);
    g->edges = (Edge*) malloc(sizeof(Edge) * edges);
    g->numEdges = edges;
    g->size = data->rows;

    int counter = 0;
    if(data->format.symmetry == SYMMETRIC){
        for(int i =0; i < data->size; i++){
            addEdge(g, counter++, data->from[i], data->to[i], data->weight[i]);
            addEdge(g, counter++, data->to[i], data->from[i], data->weight[i]);
        }
    } else {
        for(int i =0; i < data->size; i++){
            addEdge(g, counter++, data->from[i], data->to[i], data->weight[i]);
        }
    }
//    printf("%d edges added\n", counter);

    sortEdges(g);

    return g;
}

void sortEdges(Graph* g){
    qsort(g->edges, g->numEdges, sizeof(Edge), compareEdges);
    int currEdge = 0;
    int currVertice = 0;
    while(currEdge < g->numEdges && currVertice < g->size){
        Edge edge = g->edges[currEdge];
        if(edge.weight == 0){
            g->verticeLastEdgeExclusive[currVertice] = currEdge;
            currVertice++;
            currEdge++;
        } else if (edge.from > currVertice){
            while(edge.from > currVertice){
                g->verticeLastEdgeExclusive[currVertice] = currEdge;
                currVertice++;
            }
        }
        currEdge++;
    }
    if(currEdge >= g->numEdges){
        g->verticeLastEdgeExclusive[currVertice] = currEdge;
    }
}

void addEdge(Graph* g, int index, int from, int to, float weight){
    Edge e = {.from = from - 1, .to = to - 1, .weight = weight};
    g->edges[index] = e;
//    printf("added edge %d %d %f\n", from-1, to-1, weight);
}

float hasEdge(Graph* g, int from, int to){
    for(int i = 0; i < g->numEdges; i++){
        if(g->edges[i].from == from && g->edges[i].to == to){
            return g->edges->weight;
        }
    }
    return -1.0;
}

int compareEdges(const void* a, const void* b){
    Edge* edge1 = (Edge*)a;
    Edge* edge2 = (Edge*)b;
    if(edge1->weight == 0 || edge2->weight == 0){
        if(edge1->weight != 0 ){
            return -1;
        } else if (edge2->weight != 0){
            return 1;
        } else {
            return 0;
        }
    }
    if(edge1->from > edge2->from){
        return 1;
    } else if(edge1->from < edge2->from){
        return -1;
    } else {
        if(edge1->to > edge2->to){
            return 1;
        } else if (edge1->to < edge2->to){
            return -1;
        } else {
            return 0;
        }
    }
}

void printGraph(Graph* g){
    for(int i = 0; i < g->size; i++){
        for(int j = 0; j < g->size; j++){
//            printf("checking edge %d %d\n", i , j);
            float egdeWeight = hasEdge(g, i, j);
            if(egdeWeight > 0.0){
                printf("%d %d %f\n", i, j, egdeWeight);
            }
        }
    }
}

void destroyGraph(Graph* g){
    free(g->edges);
    free(g->verticeLastEdgeExclusive);
    free(g);
}
