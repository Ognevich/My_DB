#define _CRT_SECURE_NO_WARNINGS
#include "tokenizer.h"
#include "parse_util.h"
#include "parser_keywords.h"
#include <string.h>

char** tokenize(const char* input, int* count) {
    char** tokens = safe_malloc(64 * sizeof(char*));
    *count = 0;

    const char* p = input;
    char buffer[BUFFER_SIZE];
    int bi = 0;

    while (*p) {
        if (isspace((unsigned char)*p)) {
            spaceTokenize(&bi, buffer, &tokens, &p, count);
        }
        else if (strchr("(),;", *p)) {
            specialSymbolTokenize(&bi, buffer, &tokens, &p, count);
        }
        else {
            symbolTokenize(&bi, buffer, &p);
        }
    }

    if (bi > 0) {
        buffer[bi] = '\0';
        tokens[*count] = _strdup(buffer);
        (*count)++;
    }

    return tokens;
}

char* detokenize(const char** input, int count) {
    int totalLength = 0;
    for (int i = 1; i < count; i++) {
        totalLength += strlen(input[i]);
    }

    char* result = safe_malloc(totalLength + 1);

    result[0] = '\0';

    for (int i = 1; i < count; i++) {
        strcat(result, input[i]);
    }

    return result;

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