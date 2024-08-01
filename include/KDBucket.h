#ifndef KDBUCKET_H
#define KDBUCKET_H

#include <stdio.h>
#include <stdlib.h>
#include "vectors.h"
#include "storage.h"

typedef struct KDNode {
    struct KDNode* left;
    struct KDNode* right;
    Vector vec;
    INFO_TYPE information;
    int initalized;
} KDNode;

typedef struct {
    KDNode* root;
    int dim;
    int size;
} KDBucket;

KDNode* initKDNode(Vector vec, INFO_TYPE information);
void freeKDNode(KDNode* node);
void insertKDNode(KDBucket* bucket, KDNode* node);
//KDNode* recInsert(KDNode* root, KDNode* node, int depth, int dim);
//KDNode* recDelete(KDNode* root, KDNode* node, int depth, int dim);
void deleteKDNode(KDBucket* bucket, KDNode* node);
KDNode* nearestNeighbor(KDBucket* bucket, Vector* v);
//void recNN(KDNode* root, Vector* v, KDNode** best, double* bestdist, int depth);
KDNode** kNearestNeighbors(KDBucket* bucket, Vector* v, int k);
//void recKNN(KDNode* root, Vector* v, KDNode*** best, double** bestdists, int depth, int k);
KDNode** getAllNodes(KDBucket* bucket);
KDBucket* initKDBucket(int dim);
void freeKDBucket(KDBucket* bucket);
#endif