//
// Created by stach on 08.05.2020.
//

#ifndef LOUVAIN_LOUVAIN_H
#define LOUVAIN_LOUVAIN_H

float getKi(Graph* g, int vertice);

float getKiin(Graph *g, int vertice, int* cliques, int clique);

float sigmaIN(Graph* g, int* cliques, int clique);

float sigmaTOT(Graph* g, int* cliques, int clique, int vertice);

float weightsSum(Grap* g);

int sameClique(int* cliques, int vertice1, int vertice2);

float Q(Graph* g, int* cliques);

float deltaQ(Graph* g, int* cliques, int clique, int vertice);

void printCliques(Graph* g, int* cliques);

int bestClique(Graph* g, int* cliques, int vertice, float* sigmaTOTs, float m);

int canMove(int currentClique, int targetClique, int* cliqueSizes);

float edgeWeight(Graph* g, int verticeFrom, int verticeTo);

void changeEdges(Graph* g, const int* cliques, const int* minVerticeInClique);

#endif //LOUVAIN_LOUVAIN_H
