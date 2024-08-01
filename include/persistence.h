#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "storage.h"
#include "KDBucket.h"

void writeVector(Vector* v, FILE* f);
void readVector(Vector* v, FILE* f);
void writeInfo(INFO_TYPE* info, FILE* f);
void readInfo(INFO_TYPE* info, FILE* f);
void writeDBEntry(DBEntry* entry, FILE* f);
void readDBEntry(DBEntry* entry, FILE* f);
void writeVBucket(VBucket* bucket, FILE* f);
void readVBucket(VBucket* bucket, FILE* f);
void writeKDBucket(KDBucket* bucket, FILE* f);
void readKDBucket(KDBucket* bucket, FILE* f);

#endif