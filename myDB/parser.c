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

int isSemicolon(const char* word)
{
    if (word == NULL)
        return 0;

    int len = strlen(word);
    if (len == 0)
        return 0;

    while (len > 0 && isspace((unsigned char)word[len - 1])) {
        len--;
    }

    if (len > 0 && word[len - 1] == ';') {
        return 1;
    }

    return 0;
}
