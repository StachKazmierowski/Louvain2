//
// Created by stach on 09.05.2020.
//

#ifndef LOUVAIN_ALGORITHM_H
#define LOUVAIN_ALGORITHM_H

void phaseOne(Graph* g, int* cliques, float minimalGain);

void phaseTwo(Graph* g, int* cliques);


#endif //LOUVAIN_ALGORITHM_H
