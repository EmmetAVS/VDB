#ifndef STORAGE_H
#define STORAGE_H

#include <stdio.h>
#include "vectors.h"
#ifndef INFO_TYPE
#define INFO_TYPE char*
#endif
#ifndef STRUCTURE_TYPE
#define STRUCTURE_TYPE DBEntry*
#endif

typedef struct {
    Vector vec;
    INFO_TYPE information;
    double simscore;
} DBEntry;

typedef struct {
    int size;
    int capacity;
    DBEntry *data;
    char* name;
} VBucket;

typedef struct {
    VBucket* buckets;
    int capacity;
    int size;
} VectorDB;

VBucket* initVectorBucket(char* name, int capacity);
DBEntry initDBEntry(Vector vec, INFO_TYPE info);
void freeVectorBucket(VBucket* bucket);
int addDBEntry(VBucket* bucket, DBEntry entry);
void removeDBEntry(VBucket* bucket, int index);
INFO_TYPE determineMostSimilar(VBucket* bucket, Vector* v);
void sortBucket(VBucket* bucket, Vector* v);
#endif