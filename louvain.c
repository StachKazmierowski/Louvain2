//
// Created by stach on 08.05.2020.
//
#include "dataio.h"
#include "graph_utils.h"
#include "louvain.h"

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
        if(e.to != vertice && cliques[e.to] == clique){
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
        sigmaTot[i] = 0;
    }
    for(int i = 0; i < g->size; i++){
        float ki = getKi(g, i);
        sigmaTot[cliques[i]] += ki;
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


int bestClique(Graph* g, int* cliques, int vertice, float* sigmaTOTs, float m){
    float bestDelta = 0.0;
    int bestClique = -1;
    for(int  i = EDGES_IDX(g, vertice - 1); i < EDGES_IDX(g, vertice); i++){
        Edge edge = g->edges[i];
        int to = edge.to;
        int consideredClique = cliques[to];
        if(consideredClique != cliques[vertice] && consideredClique != bestClique){
            float delta = deltaQ(g, cliques, consideredClique, vertice, sigmaTOTs, m);
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

float deltaQ(Graph* g, int* cliques, int targetClique, int vertice, float* sigmaTOTs, float m){
    float kiin = getKiin(g, vertice, cliques, targetClique);
    float ki = getKi(g, vertice);
//    printf("%f %f %f %f \n", kiin, ki, sigmaTOTs[targetClique], m);
    return ( kiin - ((ki * sigmaTOTs[targetClique]) / (2.0 * m))) / m ;
}

float altDeltaQ(Graph* g, int* cliques, int targetClique, int vertice, float* sigmaTOTs, float m){
    float ki = getKi(g, vertice);
    float kiin = getKiin(g, vertice, cliques, targetClique);
    float EiwCiBezi = getKiin(g, vertice, cliques, cliques[vertice]);
    float aciBezi= sigmaTOTs[cliques[vertice]] - ki;
    float acj = sigmaTOTs[targetClique];
    float part1 = (kiin - EiwCiBezi)/m;
    float part2 = ki * (aciBezi - acj)/(2 * m * m);
    return  part1+part2;

}

float Q(Graph* g, int* cliques, float m){
    float sum = 0.0;
    for(int i = 0 ; i < g->size; i++){
        for(int j = 0; j < g->size; j++){
            if(sameClique(cliques, i, j) == 1){
                float weight = edgeWeight(g, i, j);
//                printf("%f\n", weight);
                sum += weight - (getKi(g, i) * getKi(g, i) / (2 * m));
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

void fillCliqueSizes(Graph* g, int* cliques, int* cliqueSizes){
    for(int i = 0; i < g->size; i++){
        cliqueSizes[cliques[i]]++;
    }
}

int canMove(int from, int to, int* cliqueSizes){
    printf("from %d to %d size from %d size to %d\n", from, to, cliqueSizes[from], cliqueSizes[to]);
    if(from == to){
        return 0;
    }
    if(from > to || cliqueSizes[from] > 1 || cliqueSizes[to] > 1){
        return 1;
    }
    return 0;
}

void moveClique(int size, int* cliques, int currClique, int targetClique){
    for(int i = 0; i < size; i++){
        if(cliques[i] == currClique){
            cliques[i] = targetClique;
        }
    }
}

void updateCliques(int size, int* oldCliques, int* newCliques){
    for(int i = 0; i < size; i++){
        if(oldCliques[i] != newCliques[i]){
            moveClique(size, newCliques, oldCliques[i], newCliques[i]);
        }
    }
}

int compareMoves(const void* a, const void* b){
    Move* move1 = (Move*)a;
    Move* move2 = (Move*)b;

    if(move1->gain > move2->gain){
        return -1;
    } else if( move2->gain > move1->gain){
        return 1;
    } else {
        return 0;
    }
}

void applyMoves(int* cliques, Move* moves, int numberToApply){
    for(int i = 0; i < numberToApply; i++){
        Move m = moves[i];
        cliques[m.vertice] = m.targetClique;
        printf("vertice %d to clique %d\n", m.vertice, m.targetClique);
    }
}

void sortMoves(Move* moves, int numberOfMoves){
    qsort(moves, numberOfMoves, sizeof(Move), compareMoves);
}

void updateEdges(Graph* g, const int* cliques, const int* mins){
    for(int i = 0; i < g->numEdges; i++){
        Edge* edge = g->edges + i;
        int vertice = edge->from;
        int clique = cliques[vertice];
        int targetVertice = mins[clique];
        if(vertice != targetVertice){
            edge->from = targetVertice;
        }
        edge->to = mins[cliques[edge->to]];
    }
}

int* minimalVerticesInCliques(Graph* g, int* cliques){
    int* mins = (int*) malloc(sizeof(int) * g->size);
    for(int i = 0; i < g->size; i++){
        mins[i] = -1;
    }
    for(int i = 0; i < g->size; i++){
        int clique = cliques[i];
        if(mins[clique] == -1 || mins[i] > i){
            mins[clique] = -1;
        }
    }
}
