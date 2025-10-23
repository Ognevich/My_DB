#define _CRT_SECURE_NO_WARNINGS
#include "parser.h"
#include <stdlib.h>
#include "util.h"
#include <string.h>

char** split(char* input, int* argSize)
{
    if (!input)
        return NULL;

    int numTokens = countTokensInString(input);
    if (numTokens == 0)
        return NULL;

    char** parts = malloc(sizeof(char*) * (numTokens + 1));
    if (!parts)
        return NULL;

    *argSize = numTokens;

    int i = 0;
    char* token = strtok(input, " ");
    while (token != NULL) {
        parts[i] = malloc(strlen(token) + 1);
        if (!parts[i]) {
            for (int j = 0; j < i; j++) free(parts[j]);
            free(parts);
            return NULL;
        }
        strcpy(parts[i], token);
        i++;
        token = strtok(NULL, " ");
    }
    parts[i] = NULL;
    return parts;
}