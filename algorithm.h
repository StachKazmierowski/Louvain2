//
// Created by stach on 09.05.2020.
//

#ifndef LOUVAIN_ALGORITHM_H
#define LOUVAIN_ALGORITHM_H

int phaseOne(Graph* g, int* cliques, float minimalGain, float threshold);

void phaseTwo(Graph* g, int* cliques);

void initializeOnes(int * cliquesSizes, int size);


#endif //LOUVAIN_ALGORITHM_H
