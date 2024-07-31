#include "../include/database.h"
#include "../include/KDBucket.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    KDBucket* bucket = initKDBucket(5);
    int size = 50;
    for (int i = 0; i < size; i++) {
        Vector v = initVector(5);
        float f = (float) i;
        float arr[5] = {f, f+1.0, f+2.0, f+3.0, f+4.0};
        setVectorData(&v, arr);
        char* name = (char*)malloc(20*sizeof(char));
        sprintf(name, "Name: %d", i);
        KDNode* node = initKDNode(v, name);
        insertKDNode(bucket, node);
    }

    Vector v = initVector(5);
    int f = 5;
    float arr[5] = {f, f+1.0, f+2.0, f+3.0, f+4.0};
    setVectorData(&v, arr);
    int k = 4;
    KDNode** mostsimilar = kNearestNeighbors(bucket, &v, k);
    for (int i = 0; i < k; i++) {
        printf("I: %d, ", i);
        printf("ISNULL: %d, ", mostsimilar[i] == NULL);
        printf("Info: '%s'\n", mostsimilar[i]->information);
    }

    freeKDBucket(bucket);
}