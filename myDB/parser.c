#define _CRT_SECURE_NO_WARNINGS
#include "parser.h"
#include "logger.h"
#include "commandHandler.h"
#include <stdlib.h>
#include "util.h"
#include <string.h>
#include <ctype.h>


static const char* reservedWords[] = {
    "CREATE", "DATABASE", "TABLE", "IF", "NOT", "EXISTS", "SELECT", "INSERT", "UPDATE", "DELETE"
};
static const int reservedWordsCount = sizeof(reservedWords) / sizeof(reservedWords[0]);

char** tokenize(const char* input, int* count) {
    char** tokens = malloc(64 * sizeof(char*));
    *count = 0;

    const char* p = input;
    char buffer[128];
    int bi = 0;

    while (*p) {
        if (isspace((unsigned char)*p)) {
            if (bi > 0) {
                buffer[bi] = '\0';
                tokens[*count] = _strdup(buffer);
                (*count)++;
                bi = 0;
            }
            p++;
        }
        else if (strchr("(),;", *p)) {
            if (bi > 0) {
                buffer[bi] = '\0';
                tokens[*count] = _strdup(buffer);
                (*count)++;
                bi = 0;
            }
            char sym[2] = { *p, '\0' };
            tokens[*count] = _strdup(sym);
            (*count)++;
            p++;
        }
        else {
            buffer[bi++] = *p++;
        }
    }

    if (bi > 0) {
        buffer[bi] = '\0';
        tokens[*count] = _strdup(buffer);
        (*count)++;
    }

    return tokens;
}

int isIfNotExistsUsed(char** argv, int argSize)
{
    if (argSize < 3) {
        return 0;
    }

    for (int i = 0; i < argSize-2; i++) {
        if (strcmp(argv[i], "IF") == 0 &&
            strcmp(argv[i + 1], "NOT") == 0 &&
            strcmp(argv[i + 2], "EXISTS") == 0) {
            return 1; 
        }
    }
    return 0; 
}

char* extractName(char** argv, int argc, int ifNotExists) {
    char* name = NULL;
    if (ifNotExists && argc >= 6) name = argv[5];
    else if (!ifNotExists && argc >= 3) name = argv[2];

    if (!name || isReservedWord(name))
        return NULL;
    return name;
}

int isReservedWord(const char* word)
{
    for (int i = 0; i < reservedWordsCount; i++) {
        if (strcmp(reservedWords[i], word) == 0)
            return 1;
    }
    return 0;
}
