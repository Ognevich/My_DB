#include "parse_util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

void* safe_malloc(size_t s) {
    void* p = malloc(s);
    if (!p) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    return p;
}

void* safe_realloc(void* p, size_t s) {
    void* tmp = realloc(p, s);
    if (!tmp) {
        fprintf(stderr, "Out of memory (realloc)\n");
        exit(EXIT_FAILURE);
    }
    return tmp;
}

void freeExtractedValues(char*** values, int size) {
    if (!values) return;
    for (int i = 0; i < size; i++) {
        char** row = values[i];
        if (row) {
            for (int j = 0; row[j]; j++) {
                free(row[j]);
            }
            free(row);
        }
    }
    free(values);
}


void spaceTokenize(int* bi, char* buffer, char*** tokens, const char** p, int* count)
{
    if (*bi > 0) {
        buffer[*bi] = '\0';
        (*tokens)[*count] = _strdup(buffer);
        (*count)++;
        *bi = 0;
    }
    (*p)++;
}

void specialSymbolTokenize(int* bi, char* buffer, char*** tokens, const char** p, int* count)
{
    if (*bi > 0) {
        buffer[*bi] = '\0';
        (*tokens)[*count] = _strdup(buffer);
        (*count)++;
        *bi = 0;
    }

    char sym[2] = { **p, '\0' };
    (*tokens)[*count] = _strdup(sym);
    (*count)++;
    (*p)++;
}

void symbolTokenize(int* bi, char* buffer, const char** p)
{
    buffer[(*bi)++] = **p;
    (*p)++;
}
