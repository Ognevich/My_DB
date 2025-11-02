#define _CRT_SECURE_NO_WARNINGS
#include "parser.h"
#include "logger.h"
#include "commandHandler.h"
#include <stdlib.h>
#include "util.h"
#include "config.h"
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

    if (!name || isReservedWord(name) || !isValidName(name))
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

int isValidName(const char* word)
{
    while (*word != '\0')
    {
        if (strchr(FORBIDEN_SYMBOLS, *word))
            return 0;
        word++;
    }
    return 1;
}

int isBracketsExists(const char** argv, int argc, int ifNotExists)
{
    int tableNameIndex = ifNotExists ? 5 : 2;

    if (tableNameIndex + 1 >= argc) return 0;
    if (!strchr(argv[tableNameIndex + 1], '('))
        return 0;

    if (!strchr(argv[argc - 1], ')'))
        return 0;

    return 1;
}

char*** extractInnerArgs(const char** argv, int argc, int* innerArgs) {
    if (!argv || argc <= 0 || !innerArgs) return NULL;

    int isOpenBracket = 0;
    int counter = 0;         
    int arraySize = 4;       
    char*** result = malloc(arraySize * sizeof(char**));

    if (!result) return NULL;

    char* currentName = NULL;
    char* currentType = NULL;

    for (int i = 0; i < argc; i++) {
        if (!isOpenBracket) {
            if (strcmp(argv[i], "(") == 0) {
                isOpenBracket = 1;
            }
            continue;
        }

        if (strcmp(argv[i], ")") == 0)
            break;

        if (strcmp(argv[i], ",") == 0)
            continue;

        if (!currentName) {
            currentName = (char*)argv[i];
        }
        else if (!currentType) {
            currentType = (char*)argv[i];

            char** pair = malloc(3 * sizeof(char*));
            pair[0] = _strdup(currentName);
            pair[1] = _strdup(currentType);
            pair[2] = NULL;

            result[counter++] = pair;

            currentName = NULL;
            currentType = NULL;

            if (counter >= arraySize) {
                arraySize *= 2;
                result = realloc(result, arraySize * sizeof(char**));
                if (!result) return NULL;
            }
        }
    }

    *innerArgs = counter;
    return result;
}









