//
// Created by stach on 08.05.2020.
//

#ifndef LOUVAIN_LOUVAIN_H
#define LOUVAIN_LOUVAIN_H

typedef struct {
    int vertice;
    int targetClique;
    float gain;
} Move;

int compareMoves(const void* a, const void* b);

void applyMoves(int* cliques, Move* moves, int numberToApply);

void sortMoves(Move* moves, int numberOfMoves);

float getKi(Graph* g, int vertice);

float getKiin(Graph *g, int vertice, int* cliques, int clique);

float sigmaIN(Graph* g, int* cliques, int clique);

float sigmaTOT(Graph* g, int* cliques, int clique);

float weightsSum(Graph* g);

int sameClique(int* cliques, int vertice1, int vertice2);

float Q(Graph* g, int* cliques, float m);

float deltaQ(Graph* g, int* cliques, int clique, int vertice, float* sigmaTOTs, float m);

void printCliques(Graph* g, int* cliques);

int bestClique(Graph* g, int* cliques, int vertice, float* sigmaTOTs, float m);

int canMove(int currentClique, int targetClique, int* cliqueSizes);

float edgeWeight(Graph* g, int verticeFrom, int verticeTo);

void changeEdges(Graph* g, const int* cliques, const int* minVerticeInClique);

int* cliqueSizes(Graph* g, int* cliques);

void moveClique(int size, int* cliques, int currClique, int targetClique);

void updateCliques(int size, int* oldCliques, int* newCliques);





void updateEdges(Graph* g, const int* cliques, const int* mins);

int* minimalVerticesInCliques(Graph* g, int* cliques);



#endif //LOUVAIN_LOUVAIN_H
