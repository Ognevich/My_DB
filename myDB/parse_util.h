#ifndef _PARSE_UTIL_H_
#define _PARSE_UTIL_H_

void freeInnerArgs(char*** result, int count);
void freePair(const char** pair);
void freeCharArr(const char** value, int size);
#endif
