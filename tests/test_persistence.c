#include "../include/database.h"

int testVectorSave() {
    Vector v = initVector(5);
    float arr[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
    setVectorData(&v, arr);
    FILE *f = fopen("data.bin", "wb");
    writeVector(&v, f);
    fclose(f);

    Vector v2 = initVector(5);
    FILE *f2 = fopen("data.bin", "rb");
    readVector(&v2, f2);
    fclose(f2);

    for (int i = 0; i < 5; i++) {
        assert(v.data[i] == v2.data[i]);
    }

    free(v.data);
    free(v2.data);

    return 0;
}
int testVBucketSave() {
    VBucket* bucket = initVectorBucket("Bucket", 10);
    int size = 15;
    for (int i = 0; i < size; i++) {
        float f = (float) i;
        Vector v = initVector(5);
        float arr[5] = {f, f+1.0, f+2.0, f+3.0, f+4.0};
        setVectorData(&v, arr);
        char* name = (char*)malloc(8*sizeof(char));
        sprintf(name, "Name: %d", i);
        DBEntry e = initDBEntry(v, name);
        addDBEntry(bucket, e);
    }
    
    FILE *f = fopen("data.bin", "wb");
    writeVBucket(bucket, f);
    fclose(f);

    
    VBucket* bucket2 = (VBucket*)malloc(sizeof(VBucket));
    FILE *f2 = fopen("data.bin", "rb");
    readVBucket(bucket2, f2);
    fclose(f2);

    printf("B1 capacity (%d) vs B2 capacity (%d)\n", bucket->capacity, bucket2->capacity);
    assert(bucket->capacity == bucket2->capacity);
    printf("B1 size (%d) vs B2 size (%d)\n", bucket->size, bucket2->size);
    assert(bucket->size == bucket2->size);
    printf("B1 name (%s) vs B2 name (%s)\n", bucket->name, bucket2->name);
    assert(bucket->name[0] == bucket2->name[0]);

    printf("\nEntries\n");
    for (int i = 0; i < size; i++) {
        printf("B1 Entry %d: Name: %s, Vector Dim: %d, Vec[0]: %f\n", i, bucket->data[i].information, bucket->data[i].vec.dim, bucket->data[i].vec.data[0]);
        printf("B2 Entry %d: Name: %s, Vector Dim: %d, Vec[0]: %f\n", i, bucket2->data[i].information, bucket2->data[i].vec.dim, bucket2->data[i].vec.data[0]);
        assert(bucket->data[i].information[0] == bucket2->data[i].information[0]);
        assert(bucket->data[i].vec.dim == bucket2->data[i].vec.dim);
        for (int q = 0; q < bucket->data[i].vec.dim; q++) {
            assert(bucket->data[i].vec.data[q] == bucket2->data[i].vec.data[q]);
        }
    }
    freeVectorBucket(bucket);
    freeVectorBucket(bucket2);

    return 0;
}
int testKDBucketSave() {
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

    FILE *f = fopen("data.bin", "wb");
    writeKDBucket(bucket, f);
    fclose(f);

    KDBucket* bucket2 = (KDBucket*)malloc(sizeof(KDBucket));
    FILE *f2 = fopen("data.bin", "rb");
    readKDBucket(bucket2, f2);
    fclose(f2);

    //Assertions
    assert(bucket->dim == bucket2->dim);
    assert(bucket->size == bucket2->size);

    int k = 5;
    Vector v = initVector(5);
    float i = 5.0f;
    float arr[5] = {i, i+1.0, i+2.0, i+3.0, i+4.0};
    KDNode** Bucket1_KNN = kNearestNeighbors(bucket, &v, k);
    KDNode** Bucket2_KNN = kNearestNeighbors(bucket2, &v, k);
    for (int q = 0; q < k; q++) {
        assert(Bucket1_KNN[q]->vec.dim == Bucket2_KNN[q]->vec.dim);
        for (int d = 0; d < Bucket1_KNN[q]->vec.dim; d++) {
            assert(Bucket1_KNN[q]->vec.data[d] == Bucket2_KNN[q]->vec.data[d]);
        }
    }

    return 0;
}