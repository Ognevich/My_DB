#ifndef _UTIL_H_
#define _UTIL_H_
#include "table.h"

int increaseTwoDimCharArray(char*** array, int size);
void freeTwoDimArray(void*** array, int rows);
void freeThreeDimArray(void**** array, int rows);
void printHeader(const char* headerName);
int defineColumnSize(FieldType type);

FieldType StrToField(char * filedType);

void printTokens(const char** tokens, int tokensSize);

#endif

