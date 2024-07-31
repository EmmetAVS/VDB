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
    KDBucket* bucket = initKDBucket(5);
    int size = 50;
    for (int i = 0; i < size; i++) {
        Vector v = initVector(5);
        float f = (float) i;
        float arr[5] = {f, f+1.0, f+2.0, f+3.0, f+4.0};
        setVectorData(&v, arr);
        char* name = (char*)malloc(8*sizeof(char));
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
    assert(mostsimilar[0]->vec.data[0] == 5);
    assert(mostsimilar[1]->vec.data[0] == 4);
    assert(mostsimilar[2]->vec.data[0] == 6);
    assert(mostsimilar[3]->vec.data[0] == 3);

    deleteKDNode(bucket, mostsimilar[0]);

    KDNode** mostsimilar_2 = kNearestNeighbors(bucket, &v, k);
    assert(mostsimilar_2[0]->vec.data[0] == 4);
    assert(mostsimilar_2[1]->vec.data[0] == 6);
    assert(mostsimilar_2[2]->vec.data[0] == 7);
    assert(mostsimilar_2[3]->vec.data[0] == 3);

    free(mostsimilar_2);
    free(mostsimilar);
    freeKDBucket(bucket);
    return 0;
}