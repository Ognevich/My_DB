#include "parser_create.h"
#include "parse_util.h"
#include "parser_keywords.h"
#include "commandValidators.h"
#include <string.h>
#include <stdlib.h>

int isIfNotExistsUsed(char** argv, int argSize)
{
    if (argSize < 3) {
        return 0;
    }

    for (int i = 0; i < argSize - 2; i++) {
        if (strcmp(argv[i], "IF") == 0 &&
            strcmp(argv[i + 1], "NOT") == 0 &&
            strcmp(argv[i + 2], "EXISTS") == 0) {
            return 1;
        }
    }
    return 0;
}

void extractName(char** argv, int argc, char** name, int ifNotExists) {
    *name = NULL;

    if (ifNotExists && argc >= 6) {
        *name = argv[5];
    }
    else if (!ifNotExists && argc >= 3) {
        *name = argv[2];
    }

    if (*name && (isReservedWord(*name) || hasForbiddenSymbol(*name))) {
        *name = NULL;
    }
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

int extractInnerArgs(const char** argv, int argc, char**** outResult, int* innerArgs)
{
    if (!argv || argc <= 0 || !outResult || !innerArgs)
        return 0;

    *outResult = NULL;
    *innerArgs = 0;

    int isOpenBracket = 0;
    int counter = 0;
    int arraySize = 4;

    char*** result = safe_malloc(arraySize * sizeof(char**));
    if (!result) return 0;

    char* currentName = NULL;
    char* currentType = NULL;
    int expectComma = 0;

    for (int i = 0; i < argc; i++) {
        const char* token = argv[i];

        if (!isOpenBracket) {
            if (strcmp(token, "(") == 0)
                isOpenBracket = 1;
            continue;
        }

        if (strcmp(token, ")") == 0) {
            if (currentName && !currentType) {
                fprintf(stderr, "Error: column '%s' has no data type.\n", currentName);
                freeInnerArgs(result, counter);
                return 0;
            }
            break;
        }

        if (strcmp(token, ",") == 0) {
            if (!expectComma) {
                fprintf(stderr, "Syntax error: unexpected ',' near '%s'.\n", token);
                freeInnerArgs(result, counter);
                return 0;
            }
            expectComma = 0;
            continue;
        }

        if (!expectComma) {
            if (!currentName)
                currentName = _strdup(token);
            else if (!currentType) {
                currentType = _strdup(token);

                if (!addPair(&result, &counter, &arraySize, currentName, currentType)) {
                    freeInnerArgs(result, counter);
                    return 0;
                }

                currentName = NULL;
                currentType = NULL;
                expectComma = 1;
            }
        }
        else {
            fprintf(stderr, "Syntax error: expected ',' or ')' before '%s'.\n", token);
            freeInnerArgs(result, counter);
            return 0;
        }
    }

    if (!expectComma && currentName && !currentType) {
        fprintf(stderr, "Error: column '%s' has no data type.\n", currentName);
        free(currentName);
        freeInnerArgs(result, counter);
        return 0;
    }

    *innerArgs = counter;
    *outResult = result;

    return 1;
}