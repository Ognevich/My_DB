#ifndef _UTIL_H_
#define _UTIL_H_
#include "table.h"
#include "parser_keywords.h"

int increaseTwoDimCharArray(char*** array, int size);
void freeTwoDimArray(void*** array, int rows);
void freeThreeDimArray(void**** array, int rows);
void freeParsedValues(char*** values, int valuesSize);
void printHeader(const char* headerName);
int defineColumnSize(FieldType type);

FieldType StrToField(char * filedType);
const char* sqlErrorToString(SqlError error);
void printError(SqlError error);

void printTokens(const char** tokens, int tokensSize);
void printParsedValues(char *** values,int valuesSize );

int isNumber(const char* s);
int isQuotedString(const char* s);
int isNULL(const char* s);


#endif

