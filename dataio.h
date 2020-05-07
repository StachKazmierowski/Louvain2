//
// Created by stach on 07.05.2020.
//

#ifndef LOUVAIN_DATAIO_H
#define LOUVAIN_DATAIO_H

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define COORDINATE 1
#define ARRAY 2

#define REAL 1
#define INTEGER 2
#define COMPLEX 3
#define PATTERN 4

#define GENERAL 1
#define SYMMETRIC 2
#define SKEW 3
#define HERMITIAN 4

#define THROW(msg, exit_code) printf("ERROR OCCURED\n%s\n", msg); exit(exit_code)

typedef struct {
    int format;
    int valueType;
    int symmetry;
} MFormat;

typedef struct {
    int* from;
    int* to;
    float* weight;
    int size;
    MFormat format;
    int rows;
    int cols;
} MData;


void validateFormat(MFormat form);

MFormat readSignature(FILE* filePtr);

MData* readData(const char* filename);

void printData(MData* data);

MData* initMData(int rows, int cols, int size, MFormat format);

void destroyMData(MData* mdata);


#endif //LOUVAIN_DATAIO_H
