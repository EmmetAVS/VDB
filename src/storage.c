#include "../include/storage.h"
#include "../include/database.h"
#include <stdlib.h>

VBucket* initVectorBucket(char* name, int capacity) {
    VBucket* bucket = (VBucket*)malloc(sizeof(VBucket));
    bucket->capacity = capacity;
    bucket->name = name;
    bucket->data = (DBEntry*)malloc(capacity * sizeof(DBEntry));
    bucket->size = 0;
    return bucket;
}
DBEntry initDBEntry(Vector vec, INFO_TYPE info) {
    DBEntry entry;
    entry.information = info;
    entry.vec = vec;
    return entry;
}
void freeVectorBucket(VBucket* bucket) {
    for (int i = 0; i < bucket->size; i++) {
        free(bucket->data[i].vec.data);
    }
    free(bucket->data);
    free(bucket);
}
int addDBEntry(VBucket* bucket, DBEntry entry) {
    if (bucket->capacity < bucket->size+1) {
        DBEntry* temp = (DBEntry*)realloc(bucket->data, 2*(bucket->capacity)*sizeof(DBEntry));
        if (temp == NULL) {
            //Failed to reallocate memory
            return 1;
        }
        bucket->data = temp;
        bucket->capacity = bucket->capacity * 2;
    }

    bucket->data[bucket->size] = entry;
    bucket->size += 1;
    return 0;
}
void removeDBEntry(VBucket* bucket, int index) {
    if (index >= bucket->size) {
        return;
    }
    free(bucket->data[index].vec.data);
    for (int i = index; i < bucket->size-1; i++) {
        bucket->data[i] = bucket->data[i+1];
    }
    bucket->size -= 1;
    return;
}
INFO_TYPE determineMostSimilar(VBucket* bucket, Vector* v) {
    double mostSimilar = -2.00;
    int index = 0;
    for (int i = 0; i < bucket->size; i++) {
        double simScore = cosineSimilarity(&bucket->data[i].vec, v);
        if (simScore > mostSimilar) {
            index = i;
            mostSimilar = simScore;
        }
    }

    return bucket->data[index].information;
}

void sortBucket(VBucket* bucket, Vector* v) {
    //Calculate original simsores and save them
    for (int i = 0; i < bucket->size; i++) {
        bucket->data[i].simscore = cosineSimilarity(&bucket->data[i].vec, v);
    }

    //Sort the list based on the pre-calculated sim scores
    int sorted = 0;
    while (!sorted) {
        sorted = 1;
        double prevScore = bucket->data[0].simscore;
        for (int i = 1; i < bucket->size; i++) {
            double simScore = bucket->data[i].simscore;
            if (simScore > prevScore) {
                sorted = 0;
                DBEntry old = bucket->data[i-1];
                bucket->data[i-1] = bucket->data[i];
                bucket->data[i] = old;
                prevScore = simScore;
            }
        }
    }
}