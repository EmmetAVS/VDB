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
    return db;
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