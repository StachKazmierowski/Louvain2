//
// Created by stach on 08.05.2020.
//
#include "dataio.h"
#include "graph_utils.h"

float getKi(Graph* g, int vertice){
    float sum = 0.0;
    for(int i = EDGES_IDX(g,vertice-1); i < EDGES_IDX(g,vertice); i++){
//        printf("%d\n", i);
        sum += g->edges[i].weight;
    }
    return sum;
}

float getKiin(Graph *g, int vertice, int* cliques, int clique){
    float sum = 0.0;
    for(int i = EDGES_IDX(g,vertice-1); i < EDGES_IDX(g,vertice); i++){
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

float sigmaTOT(Graph* g, int* cliques, int clique){
    float sum = 0.0;
    for(int i = 0; i < g->numEdges; i++){
        Edge e = g->edges[i];
        if(cliques[e.to] == clique || cliques[e.from] == clique){
            sum += e.weight;
        }
    }
    return sum / 2;
}

void recalcSigmaTOTS(Graph*g, float* sigmaTot, int* cliques){
    for(int i = 0; i < g->size; i++){
        sigmaTot[i] = 0.0;
    }
    for(int i =0 ; i < g->numEdges; i++){
        Edge e = g->edges[i];
        int cliqueFrom = cliques[e.from];
        int cliqueTo = cliques[e.to];
        if(cliqueFrom == cliqueTo){
            sigmaTot[cliqueFrom] += e.weight / 2;
        } else {
            sigmaTot[cliqueFrom] += e.weight / 2;
            sigmaTot[cliqueTo] += e.weight / 2;
        }
    }
}

void printCliques(Graph* g, int* cliques){
    for(int i = 0; i < g->size; i++){
        printf("%d %d\n", i, cliques[i]);
    }
}

float weightsSum(Graph* g){
    float sum = 0.0;
    for(int i = 0; i < g->numEdges; i++){
        Edge e = g->edges[i];
        sum += e.weight;
    }
    return sum / 2;
}

float deltaQ(Graph* g, int* cliques, int clique, int vertice){
    float kiin = getKiin(g, vertice, cliques, clique);
    float ki = getKi(g, vertice);
    float sigmaT = sigmaTOT(g, cliques, clique);
    float m = weightsSum(g);
    printf("%f %f %f %f \n", kiin, ki, sigmaT, m);
    return ( kiin - ((ki * sigmaT) / (2 * m))) / m ;
}

int bestClique(Graph* g, int* cliques, int vertice, float* sigmaTOTs, float m){
    float bestDelta = 0.0;
    int bestClique = -1;
    for(int  i = EDGES_IDX(g, vertice - 1); i < EDGES_IDX(g, vertice); i++){
        Edge edge = g->edges[i];
        int to = edge.to;
        int consideredClique = cliques[to];
        if(consideredClique != cliques[vertice] && consideredClique != bestClique){
            float delta = deltaQ(g, cliques, consideredClique, vertice);
            if(delta > bestDelta){
                bestClique = consideredClique;
                bestDelta = delta;
            } else if (delta == bestClique && bestClique > consideredClique){
                bestClique = consideredClique;
            }
        }
    }
    return bestClique;
}

//int canMove(int currentClique, int targetClique, int* cliqueSizes){
//    if(cu)
//}

int sameClique(int* cliques, int vertice1, int vertice2){
    if(cliques[vertice1] == cliques[vertice2]){
        return 1;
    } else {
        return -1;
    }
}

float edgeWeight(Graph* g, int verticeFrom, int verticeTo){
    for(int  i = EDGES_IDX(g, verticeFrom - 1); i < EDGES_IDX(g, verticeFrom); i++){
        if(g->edges[i].to == verticeTo){
            return g->edges[i].weight;
        }
    }
    return 0.0;
}

float Q(Graph* g, int* cliques, float m){
    float sum = 0.0;
    for(int i = 0 ; i < g->size; i++){
        for(int j = 0; j < g->size; j++){
            if(sameClique(cliques, i, j) == 1){
                float weight = edgeWeight(g, i, j);
//                printf("%f\n", weight);
                sum += weight - (getKi(g, i) * getKi(g, j) / (2 * m));
            }
        }
    }
    return sum / (2 * m);
}

void changeEdges(Graph* g, const int* cliques, const int* minVerticeInClique){
    for(int i =0 ; i < g->numEdges; i++){
        Edge* edge = g->edges + i;
        int from = edge->from;
        int to = edge->to;
        int superVerticeFrom = minVerticeInClique[cliques[from]];
        int superVerticeTo = minVerticeInClique[cliques[to]];
        edge->from = superVerticeFrom;
        edge->to = superVerticeTo;
    }
}

