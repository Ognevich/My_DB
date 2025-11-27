#ifndef _PARSE_UTIL_H_
#define _PARSE_UTIL_H_

void freeInnerArgs(char*** result, int count);
void freePair(const char** pair);
void freeCharArr(const char** value, int size);
void* safe_malloc(size_t s);
void* safe_realloc(void * p, size_t s);

#endif
