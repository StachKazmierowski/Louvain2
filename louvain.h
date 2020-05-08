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



#endif //LOUVAIN_LOUVAIN_H
