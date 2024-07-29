#include "../include/database.h"

int main() {
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

    printf("\n");
    removeDBEntry(bucket, 0);
    for (int i = 0; i < bucket->size; i++) {
        printf("Entry %d: '%s', Vec 1: %f\n", i, bucket->data[i].information, bucket->data[i].vec.data[0]);
    }
}