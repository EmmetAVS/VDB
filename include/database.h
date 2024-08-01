#ifndef DATABASE_H
#define DATABASE_H
#include "storage.h"
#include "KDBucket.h"
#include "persistence.h"

VectorDB* initVectorDB(int capacity);
void freeVectorDB(VectorDB* db);
void removeVectorBucket(VectorDB* db, char* name);
#endif