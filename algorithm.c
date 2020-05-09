//
// Created by stach on 09.05.2020.
//

#include "algorithm.h"
//#include "louvain.c"
void initializeOnes(int * cliquesSizes, int size){
    for(int i = 0; i < size; i++){
        cliquesSizes[i] = 1;
    }
}

float previewQ(Graph * g, int*newCliques, Move* moves, int nMoves, float m){
    applyMoves(newCliques, moves, nMoves);
    float newMod = Q(g, newCliques, m);
    return newMod;
}

int phaseOne(Graph* g, int* cliques, float minGain, float threshold){
    //Getting ready
    int changed = 1;
    int iters = 0;
    float* sigmaTOTs = (float*) malloc(sizeof(float) * g->size);
    int* cliquesSizes = (int*) calloc(sizeof(int), g->size);
    initializeOnes(cliquesSizes, g->size);

    for(int i = 0 ; i < g->size ; i++){
        printf("%d\n", cliquesSizes[i]);
    }

    Move* moves = (Move*) calloc(sizeof(Move), g->size);
    int movesDone = 0;
    float m = weightsSum(g);
    float modularity = Q(g, cliques, m);

    recalcSigmaTOTS(g, sigmaTOTs, cliques);

    while(changed != 0){
        printf("================================iter: %d================================\n", iters);
        printf("modularity %f\n", modularity);
        changed = 0;
        iters++;
        movesDone = 0;
        for(int vertice = 0; vertice < g->size; vertice++) {
            int targetClique = bestClique(g, cliques, vertice, sigmaTOTs, m);
            if (targetClique != -1) {
                float delta = deltaQ(g, cliques, targetClique, vertice, sigmaTOTs, m);
                if (delta > minGain && canMove(cliques[vertice], targetClique, cliquesSizes) == 1) {
                    printf("moving vertice %d from: %d to: %d\n", vertice, cliques[vertice], targetClique);
                    changed = 1;
                    int oldClique = cliques[vertice];
                    Move *move = moves + movesDone;
                    move->vertice = vertice;
                    move->gain = delta;
                    move->targetClique = targetClique;
                    movesDone++;
                    cliquesSizes[targetClique]++;
                    cliquesSizes[oldClique]--;
                }
            }
        }
        sortMoves(moves, movesDone);

        int* newCliques = malloc(sizeof(int) * g->size);
        memcpy(newCliques, cliques, sizeof(int) * g->size);
        float  newModularity = previewQ(g, newCliques, moves, movesDone, m);
        printf("modularity gain %f\n", newModularity - modularity);
        printCliques(g, cliques);
        printCliques(g, newCliques);

        if(movesDone > 0 && newModularity - modularity > threshold){
                memcpy(cliques, newCliques, sizeof(int) * g->size);
                recalcSigmaTOTS(g, sigmaTOTs, cliques);
                modularity = newModularity;
        } else {
            changed = 0;
        }
        free(newCliques);
        if(iters > 10){
            break;
        }
    }
    free(cliquesSizes);
    free(moves);
    free(sigmaTOTs);
    return iters;
}

void phaseTwo(Graph* g, int* cliques){

}
