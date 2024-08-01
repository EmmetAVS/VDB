#include "../include/persistence.h"
#include <string.h>

void writeVector(Vector* v, FILE* f) {
    fwrite(&(v->dim), sizeof(int), 1, f);
    fwrite(v->data, sizeof(VECTOR_TYPE), v->dim, f);
}

void readVector(Vector* v, FILE* f) {
    fread(&(v->dim), sizeof(int), 1, f); 
    v->data = (VECTOR_TYPE*)malloc(v->dim * sizeof(VECTOR_TYPE)); 
    if (v->data == NULL) {
        exit(EXIT_FAILURE);
    }
    fread(v->data, sizeof(VECTOR_TYPE), v->dim, f); 
}

void writeInfo(INFO_TYPE* info, FILE* f) {
    size_t len = strlen(*info); 
    fwrite(&len, sizeof(size_t), 1, f);
    fwrite(*info, sizeof(char), len, f);
}

void readInfo(INFO_TYPE* info, FILE* f) {
    size_t len;
    fread(&len, sizeof(size_t), 1, f);
    *info = (INFO_TYPE)malloc((len + 1) * sizeof(char));
    fread(*info, sizeof(char), len, f);
    (*info)[len] = '\0';
}

void writeDBEntry(DBEntry* entry, FILE* f) {
    writeVector(&entry->vec, f);
    writeInfo(&entry->information, f);
}

void readDBEntry(DBEntry* entry, FILE* f) {
    readVector(&entry->vec, f);
    readInfo(&entry->information, f);
}

void writeVBucket(VBucket* bucket, FILE* f) {
    writeInfo(&(bucket->name), f);
    fwrite(&(bucket->capacity), sizeof(int), 1, f);
    fwrite(&(bucket->size), sizeof(int), 1, f);
    for (int i = 0; i < bucket->size; i++) {
        writeDBEntry(&(bucket->data[i]), f);
    }
}

void readVBucket(VBucket* bucket, FILE* f) {
    readInfo(&(bucket->name), f);
    fread(&(bucket->capacity), sizeof(int), 1, f);
    fread(&(bucket->size), sizeof(int), 1, f);
    bucket->data = (DBEntry*)malloc(bucket->capacity * sizeof(DBEntry));
    for (int i = 0; i < bucket->size; i++) {
        DBEntry* e = (DBEntry*)malloc(sizeof(DBEntry));
        readDBEntry(e, f);
        bucket->data[i] = *e;
    }
}

void writeKDBucket(KDBucket* bucket, FILE* f) {
    fwrite(&(bucket->dim), sizeof(int), 1, f);
    fwrite(&(bucket->size), sizeof(int), 1, f);
    KDNode** nodes = getAllNodes(bucket);
    for (int i = 0; i < bucket->size; i++) {
        writeVector(&(nodes[i]->vec), f);
        writeInfo(&(nodes[i]->information), f);
    }
}

void readKDBucket(KDBucket* bucket, FILE* f) {
    int size;
    fread(&(bucket->dim), sizeof(int), 1, f);
    fread(&size, sizeof(int), 1, f);
    for (int i = 0; i < size; i++) {
        Vector v;
        readVector(&v, f);
        INFO_TYPE info = (INFO_TYPE)malloc(sizeof(char));
        readInfo(&info, f);
        insertKDNode(bucket, initKDNode(v, info));
    }
}