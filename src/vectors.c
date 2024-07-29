#include "../include/vectors.h"
#include <stdlib.h>
#include <math.h>

Vector initVector(int dim) {
    Vector v;
    v.dim = dim;
    v.data = (VECTOR_TYPE*)malloc(dim * sizeof(VECTOR_TYPE));
    return v;
}

void setVectorData(Vector* v, VECTOR_TYPE arr[]) {
    if (!v || !arr) {
        return;
    }
    for (int i = 0; i < v->dim; i++) {
        v->data[i] = arr[i];
    }
}

double dotProduct(Vector* v1, Vector* v2) {
    double sum = 0;
    for (int i = 0; i < v1->dim; i++) {
        sum += ((double) v1->data[i]) * ((double) v2->data[i]);
    }
    return sum;
}

double norm(Vector* v) {
    double sum = 0;
    for (int i = 0; i < v->dim; i++) {
        sum += ((double) v->data[i]) * ((double) v->data[i]);
    }
    return sqrt(sum);
}

double cosineSimilarity(Vector* v1, Vector* v2) {
    if (v1->dim != v2->dim) {
        return INVALID_DIM;
    }
    return dotProduct(v1, v2) / (norm(v1) * norm(v2));
}