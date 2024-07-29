#ifndef DATABASE_H
#define DATABASE_H
#include "storage.h"

VectorDB* initVectorDB(int capacity);
void freeVectorDB(VectorDB* db);
void removeVectorBucket(VectorDB* db, int index);
#endif