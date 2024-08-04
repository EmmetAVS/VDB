#include "../include/database.h"
#define exp __declspec(dllexport)

//gcc -shared -o VectorDB.dll src/database.c src/KDBucket.c src/persistence.c src/storage.c src/vectors.c src/DLL_Bindings.c -Wall -DBUILDING_DLL

exp KDBucket* createKDBucket(int dim) {
    return initKDBucket(dim);
}
exp void deleteKDBucket(KDBucket* bucket) {
    freeKDBucket(bucket);
}
exp void saveKDBucket(KDBucket* bucket, char* filename) {
    FILE *f = fopen(filename, "wb");
    writeKDBucket(bucket, f);
    fclose(f);
}
exp KDBucket* loadKDBucket(char* filename) {
    KDBucket* bucket = (KDBucket*)malloc(sizeof(KDBucket));
    FILE *f = fopen(filename, "rb");
    readKDBucket(bucket, f);
    fclose(f);
    return bucket;
}
exp Vector createVector(int dim, VECTOR_TYPE* data) {
    Vector v = initVector(dim);
    setVectorData(&v, data);
    return v;
}
exp void deleteVector(Vector v) {
    free(v.data);
}
exp KDNode* createKDNode(char* info, Vector v) {
    return initKDNode(v, info);
}
exp void KDBucket_Add(KDBucket* bucket, KDNode* node) {
    insertKDNode(bucket, node);
}
exp void KDBucket_Delete(KDBucket* bucket, KDNode* node) {
    deleteKDNode(bucket, node);
}
exp KDNode** KDBucket_KNN(KDBucket* bucket, Vector v, int k) {
    if (k > bucket->size) {
        return kNearestNeighbors(bucket, &v, bucket->size);    
    }
    return kNearestNeighbors(bucket, &v, k);
}
exp void Test() {
    printf("Activated\n");
}
exp INFO_TYPE parseInfo(KDNode* node) {
    if (!node) {
        printf("Returning NUll\n");
        return NULL;
    }
    const int length = strlen(node->information);
    char* info = malloc(sizeof(char) * (length+1));
    for (int i = 0; i < length; i ++) {
        info[i] = (node->information)[i];
    }
    info[length] = 0;
    return info;
}