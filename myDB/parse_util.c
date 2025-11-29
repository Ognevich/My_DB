#define _CRT_SECURE_NO_WARNINGS
#include "parse_util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char* reservedWords[] = {
    "CREATE", "DATABASE", "TABLE", "IF", "NOT", "EXISTS", "SELECT", "INSERT", "UPDATE", "DELETE"
};

const int reservedWordsCount = sizeof(reservedWords) / sizeof(reservedWords[0]);

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

int isReservedWord(const char* word)
{
    for (int i = 0; i < reservedWordsCount; i++) {
        if (strcmp(reservedWords[i], word) == 0)
            return 1;
    }
    return 0;
}
int addPair(char**** resultPtr, int* count, int* capacity, const char* name, const char* type) {
    char*** result = *resultPtr;

    if (*count >= *capacity) {
        *capacity *= 2;
        char*** tmp = safe_realloc(result, (*capacity) * sizeof(char**));
        result = tmp;
        *resultPtr = result;
    }

    char** pair = safe_malloc(3 * sizeof(char*));

    pair[0] = _strdup(name);
    pair[1] = _strdup(type);
    pair[2] = NULL;

    if (!pair[0] || !pair[1]) {
        freePair(pair);
        return 0;
    }

    result[*count] = pair;
    (*count)++;
    return 1;
}

int isKeyWordInArray(const char** argv, int argc)
{
    for (int i = 0; i < argc; i++) {

        for (int k = 0; k < reservedWordsCount; k++) {

            if (strcmp(argv[i], reservedWords[k]) == 0) {
                return 1;
            }
        }

    }
    return 0;
}

char* copyString(const char* src) {
    char* dst = safe_malloc(strlen(src) + 1);
    if (dst) strcpy(dst, src);
    return dst;
}

char** resizeRow(char** row, int* capacity) {
    int newCapacity = (*capacity) * 2;
    char** tmp = safe_realloc(row, sizeof(char*) * newCapacity);
    *capacity = newCapacity;
    return tmp;
}

int expectChar(const char** argv, int argc, int index, const char* expected) {
    if (index >= argc || strcmp(argv[index], expected) != 0) {
        printf("Syntax error: expected '%s'\n", expected);
        return 0;
    }
    return 1;
}
