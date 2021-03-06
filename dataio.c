//
//
//
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "dataio.h"

void validateFormat(MFormat form){
    int dat = form.format;
    int val = form.valueType;
    int sym = form.symmetry;
    if(sym == HERMITIAN && val != COMPLEX){
        THROW("validateFormat: hermitian must be complex!", 1);
    }
    if(val == PATTERN && sym != SYMMETRIC && sym != GENERAL && dat != COORDINATE){
        THROW("validateFormat: invalid data structure and/or symetris for type pattern", 2);
    }
}

MFormat readSignature(FILE* filePtr){
    MFormat ret;
    char formatStr[20];
    char valueTypeStr[20];
    char symmetryStr[20];

    int a = fscanf(filePtr, "%*s %*s %s", formatStr);
    int b = fscanf(filePtr, "%s", valueTypeStr) == 1;
    int c = fscanf(filePtr, "%s\n", symmetryStr) == 1;

    if(a==1 && b==1 && c==1){
        printf("readSignature: read format %s %s %s\n", formatStr, valueTypeStr, symmetryStr);
    } else {
        printf("readSignature: invalid file or format\n");
        exit(1);
    }

    if(strcmp("coordinate", formatStr) == 0){
        ret.format = COORDINATE;
    } else if(strcmp("array", formatStr) == 0){
        ret.format = ARRAY;
    } else {
        THROW("readSignature: invalid data structure!", 2);
    }

    if (strcmp("real", valueTypeStr) == 0){
        ret.valueType = REAL;
    } else if (strcmp("integer", valueTypeStr) == 0){
        ret.valueType = INTEGER;
    } else if (strcmp("complex", valueTypeStr) == 0){
        ret.valueType = COMPLEX;
    } else if (strcmp("pattern", valueTypeStr) == 0){
        ret.valueType = PATTERN;
    } else {
        THROW("readSignature: invalid value type!", 3);
    }

    if (strcmp("general", symmetryStr) == 0){
        ret.symmetry = GENERAL;
    } else if (strcmp("symmetric", symmetryStr) == 0){
        ret.symmetry = SYMMETRIC;
    } else if (strcmp("skew-symmetric", symmetryStr) == 0){
        ret.symmetry = SKEW;
    } else if (strcmp("hermitian", symmetryStr) == 0){
        ret.symmetry = HERMITIAN;
    } else {
        THROW("readSignature: invalid symetries type", 4);
    }
    validateFormat(ret);
    return ret;
}

MData* readData(const char* filename){
    FILE* filePtr = fopen(filename, "r");
    if (filePtr == NULL){
        THROW("readData: no such file", 10);
    }

    MFormat format = readSignature(filePtr);
    char line[1024];
    int num_cols, num_rows, num_lines;
    while(fscanf(filePtr, "%1024[^\n]\n", line) != EOF){
        if(line[0] != '%' && line[0] != '\000') {
            int success = -1;
            if(format.format == COORDINATE){
                success = sscanf(line, "%d %d %d", &num_rows, &num_cols, &num_lines);
            } else {
                success = sscanf(line, "%d %d", &num_rows, &num_cols);
                num_lines = num_cols * num_rows;
            }

            if(success !=3 && success != 2){
                THROW("readData: error reading matrix size!", 11);
            }
            break;
        }
    }

    printf("reading rows=%d cols=%d lines=%d\n", num_rows, num_cols, num_lines);

    MData* data = initMData(num_rows, num_cols, num_lines, format);

    for(int i = 0; i < num_lines; i++) {
//        printf("reading line %d\n", i);
        int success = -1;
        if (format.format == ARRAY) {
            data->from[i] = i / num_cols;
            data->to[i] = i - (data->from[i] * num_cols);
            success = fscanf(filePtr, "%f\n", data->weight + i);
        } else {
            if (format.valueType == PATTERN) {
                success = fscanf(filePtr, "%d %d\n", data->from + i, data->to + i);
                data->weight[i] = 1.0;
            } else {
                success = fscanf(filePtr, "%d %d %f\n", data->from + i, data->to + i, data->weight + i);
            }
        }
        if (success == -1) {
            THROW("readData: error reading rows", 13);
        }
    }
    return data;
}

void printData(MData* data){
    if(data->format.format == ARRAY){
        THROW("printing arrays not supported!", 14);
    }

    printf("%d %d %d\n", data->rows, data->cols, data->size );

    if(data->format.valueType == PATTERN){
        for(int i = 0; i < data->size; i++) {
            printf("%d %d\n", data->from[i], data->to[i]);
        }
    } else {
        for(int i = 0; i < data->size; i++) {
            printf("%d %d %f\n", data->from[i], data->to[i], data->weight[i]);
        }
    }
}

MData* initMData(int rows, int cols, int size, MFormat format){
    MData* ret = (MData*) malloc(sizeof(MData));
    ret->from = (int*) malloc(sizeof(int) * size);
    ret->to = (int*) malloc(sizeof(int) * size);
    ret->weight = (float*) malloc(sizeof(int) * size);
    ret->size = size;
    ret->format = format;
    ret->rows = rows;
    ret->cols = cols;
    return ret;
}

void destroyMData(MData* mdata){
    free(mdata->from);
    free(mdata->to);
    free(mdata->weight);
    free(mdata);
}

