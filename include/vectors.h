#ifndef VECTORS_H
#define VECTORS_H
#include <stdlib.h>
#include <math.h>
#ifndef VECTOR_TYPE
#define VECTOR_TYPE float
#endif
#define INVALID_DIM 1.1

typedef struct {
    int dim;
    VECTOR_TYPE *data;
} Vector;

Vector initVector(int dim);
void setVectorData(Vector* v, VECTOR_TYPE arr[]);
double cosineSimilarity(Vector* v1, Vector* v2);
double dotProduct(Vector* v1, Vector* v2);
double norm(Vector* v);
double distSquared(Vector* v1, Vector* v2);
#endif