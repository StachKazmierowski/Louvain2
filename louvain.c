//
// Created by stach on 08.05.2020.
//
#include "dataio.h"
#include "graph_utils.h"

float getKi(Graph* g, int vertice){
    float sum = 0.0;
    int endIndex = EDGES_IDX(g, vertice);
    for(int i = EDGES_IDX(g, vertice - 1); i <  endIndex; i++){
        sum += g->edges[i].weight;
    }
    return sum;
}

float getKiin(Graph *g, int vertice, int* cliques, int clique){
    float sum = 0.0;
    int endIndex = EDGES_IDX(g, vertice);
    for(int i = 0; i < endIndex; i++){
        Edge e = g->edges[i];
        if(cliques[e.to] == clique){
            sum += e.weight;
        }
    }
    return sum;
}

float sigmaIN(Graph* g, int* cliques, int clique){
    float sum = 0.0;
    for(int i = 0; i < g->numEdges; i++){
        Edge e = g->edges[i];
        if(cliques[e.from] == clique && cliques[e.to] == clique){
            sum += e.weight;
        }
    }
    return sum / 2;
}

float sigmaTOT(Graph* g, int* cliques, int clique, int vertice){
    float sum = 0.0;
    for(int i = 0; i < g->numEdges; i++){
        Edge e = g->edges[i];
        if(e.from == vertice && cliques[e.to] == clique){
            sum += e.weight;
        }
    }
    return sum;
}

float weightsSum(Graph* g){
    float sum = 0.0;
    for(int i = 0; i < g->numEdges; i++){
        Edge e = g->edges[i];
        sum += e.weight;
    }
    return sum / 2;
}

int sameClique(int* cliques, int vertice1, int vertice2){
    if(cliques[vertice1] == cliques[vertice2]){
        return 1;
    } else {
        return -1;
    }
}

float Q(Graph* g, int* cliques){
    float sum = 0.0;
    float m_twice = 2 * weightsSum(g);
    for(int i = 0; i < g->numEdges; i++){
        Edge e = g->edges[i];
        int from = e.from;
        int to = e.to;
        if(sameClique(cliques, from, to) == 1){
            sum += e.weight - (getKi(g, from) * getKi(g, to) /  m_twice);
        }
    }
    return sum / m_twice;
}

float deltaQ(Graph* g, int* cliques, int clique, int vertice){
    float kiin = getKiin(g, vertice, cliques, clique);
    float ki = getKi(g, vertice);
    float sigmaT = sigmaTOT(g, cliques, clique, vertice);
    float m = weightsSum(g);
//    printf("%f %f %f %f \n", kiin, ki, sigmaT, m);
    return (kiin - ki * sigmaT) / m;
}