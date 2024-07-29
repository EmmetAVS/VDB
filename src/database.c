#include "../include/database.h"
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
void removeVectorBucket(VectorDB* db, int index) {
    if (index >= db->size) {
        return;
    }
    for (int i = index; i < db->size-1; i++) {
        db->buckets[i] = db->buckets[i+1];
    }
    db->size -= 1;
    return;
}
int main()
{
    int size = 10;
    VBucket* bucket = initVectorBucket("Bucket", size);
    for (int i = 0; i < size; i++) {
        Vector v = initVector(5);
        float f = (float) i;
        float arr[5] = {f, f+1.0, f+2.0, f+3.0, f+4.0};
        setVectorData(&v, arr);
        char* name = (char*)malloc(20*sizeof(char));
        sprintf(name, "DBEntry: %d", i);
        DBEntry entry = initDBEntry(v, name);
        addDBEntry(bucket, entry);
    }

    for (int i = 0; i < size; i++) {
        printf("Entry %d: '%s', Vec 1: %f\n", i, bucket->data[i].information, bucket->data[i].vec.data[0]);
    }

    Vector v = initVector(5);
    float i = 9.0;
    float arr[5] = {i, i+1.0, i+2.0, i+3.0, i+4.0};
    setVectorData(&v, arr);
    sortBucket(bucket, &v);
    printf("\n");
    for (int i = 0; i < bucket->size; i++) {
        printf("Entry %d: '%s', Vec 1: %f\n", i, bucket->data[i].information, bucket->data[i].vec.data[0]);
    }
}