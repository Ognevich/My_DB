#ifndef _PARSE_UTIL_H_
#define _PARSE_UTIL_H_

void freeInnerArgs(char*** result, int count);
void freePair(const char** pair);
void freeCharArr(const char** value, int size);
void* safe_malloc(size_t s);
void* safe_realloc(void * p, size_t s);
void freeExtractedValues(char*** values, int size);

void spaceTokenize(int* bi, char* buffer, char*** tokens, const char** p, int* count);
void specialSymbolTokenize(int* bi, char* buffer, char*** tokens, const char** p, int* count);
void symbolTokenize(int* bi, char* buffer, const char** p);

int isReservedWord(const char* word);
int addPair(char**** resultPtr, int* count, int* capacity, const char* name, const char* type);
int isKeyWordInArray(const char** argv, int argc);

char* copyString(const char* src);
char** resizeRow(char** row, int* capacity);
int expectChar(const char** argv, int argc, int index, const char* expected);
#endif
