#include "parse_util.h"
#include <stdlib.h>

void freeInnerArgs(char*** result, int count) {
    if (!result) return;
    for (int i = 0; i < count; i++) {
        free(result[i][0]);
        free(result[i][1]);
        free(result[i]);
    }
    free(result);
}

void freePair(const char** pair) {
    free(pair[0]);
    free(pair[1]);
    free(pair);
}

void freeCharArr(const char** value, int size)
{
    for (int i = 0; i < size; i++)
        free(value[i]);
    free(value);
}

