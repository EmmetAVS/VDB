#include "../include/database.h"
#include "../include/KDBucket.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

VectorDB* initVectorDB(int capacity) {
    VectorDB* db = (VectorDB*)malloc(sizeof(VectorDB));
    db->buckets = (VBucket*)malloc(capacity * sizeof(VBucket));
    db->capacity = capacity;
    db->size = 0;
}
void freeVectorDB(VectorDB* db) {
    for (int i = 0; i < db->size; i++) {
        freeVectorBucket(&db->buckets[i]);
    }
    free(db->buckets);
    free(db);
}
void removeVectorBucket(VectorDB* db, char* name) {
    int index = -1;

    for (int i = 0; i < db->size; i++) {
        if (db->buckets[i].name == name) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return;
    }
    freeVectorBucket(&db->buckets[index]);
    for (int i = index; i < db->size-1; i++) {
        db->buckets[i] = db->buckets[i+1];
    }
    db->size -= 1;
    return;
}
int main() {
    int dim = 500;
    KDBucket* bucket = initKDBucket(dim);
    int size = 10000;

    clock_t begin = clock();

    for (int i = 0; i < size; i++) {
        Vector v = initVector(dim);
        float f = (float) i;
        float* arr = (float*)malloc(dim * sizeof(float));
        for (int d = 0; d < dim; d++) {
            arr[d] = ((float) d) + ((float) i);
        }
        setVectorData(&v, arr);
        char* name = (char*)malloc(8*sizeof(char));
        sprintf(name, "Name: %d", i);
        KDNode* node = initKDNode(v, name);
        insertKDNode(bucket, node);
    }


    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Spent %lf seconds to add %d KDNodes with %d dimensions.\n", time_spent, size, dim);
    printf("Average of %lf milliseconds per insertion\n", ((double) 1000) * (time_spent/((double) size)));
    printf("\n");

    clock_t begin2 = clock();
    FILE* f = fopen("data.bin", "wb");
    writeKDBucket(bucket, f);
    fclose(f);

    clock_t end2 = clock();
    double time_spent2 = (double)(end2 - begin2) / CLOCKS_PER_SEC;
    printf("Spent %lf seconds to save\n", time_spent2);
    printf("\n");

    clock_t begin3 = clock();
    KDBucket* bucket2 = (KDBucket*)malloc(sizeof(KDBucket));
    FILE* f2 = fopen("data.bin", "rb");
    readKDBucket(bucket, f2);
    fclose(f2);
    clock_t end3 = clock();
    double time_spent3 = (double)(end3 - begin3) / CLOCKS_PER_SEC;
    printf("Spent %lf seconds to read\n", time_spent3);
    return 0;
}