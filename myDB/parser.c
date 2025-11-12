#define _CRT_SECURE_NO_WARNINGS
#include "parser.h"
#include "logger.h"
#include "commandHandler.h"
#include "commandValidators.h"
#include <stdlib.h>
#include <stdio.h>
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

    if (!name || isReservedWord(name) || hasForbiddenSymbol(name))
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

static void freeInnerArgs(char*** result, int count) {
    if (!result) return;
    for (int i = 0; i < count; i++) {
        free(result[i][0]);
        free(result[i][1]);
        free(result[i]);
    }
    free(result);
}

static int addPair(char**** resultPtr, int* count, int* capacity, const char* name, const char* type) {
    char*** result = *resultPtr;

    if (*count >= *capacity) {
        *capacity *= 2;
        char*** tmp = realloc(result, (*capacity) * sizeof(char**));
        if (!tmp) return 0;
        result = tmp;
        *resultPtr = result;
    }

    char** pair = malloc(3 * sizeof(char*));
    if (!pair) return 0;

    pair[0] = _strdup(name);
    pair[1] = _strdup(type);
    pair[2] = NULL;

    if (!pair[0] || !pair[1]) {
        free(pair[0]);
        free(pair[1]);
        free(pair);
        return 0;
    }

    result[*count] = pair;
    (*count)++;
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
                return NULL;
            }
            break;
        }

        if (strcmp(token, ",") == 0) {
            if (!expectComma) {
                fprintf(stderr, "Syntax error: unexpected ',' near '%s'.\n", token);
                freeInnerArgs(result, counter);
                return NULL;
            }
            expectComma = 0;
            continue;
        }

        if (!expectComma) {
            if (!currentName)
                currentName = (char*)token;
            else if (!currentType) {
                currentType = (char*)token;

                if (!addPair(&result, &counter, &arraySize, currentName, currentType)) {
                    freeInnerArgs(result, counter);
                    return NULL;
                }

                currentName = NULL;
                currentType = NULL;
                expectComma = 1;
            }
        }
        else {
            fprintf(stderr, "Syntax error: expected ',' or ')' before '%s'.\n", token);
            freeInnerArgs(result, counter);
            return NULL;
        }
    }

    if (!expectComma && currentName && !currentType) {
        fprintf(stderr, "Error: column '%s' has no data type.\n", currentName);
        freeInnerArgs(result, counter);
        return NULL;
    }
    *innerArgs = counter;
    return result;
}

char** extractSelectList(const char** argv, int argc, int* listArgs)
{
    int columnCount = 0;
    int capacity = 4;
    char** selectList = malloc(capacity * sizeof(char*));
    if (!selectList)
        return NULL;

    int expectComma = 0;      
    int foundFromKeyword = 0; 
    int isValidSyntax = 1;    

    for (int i = 1; i < argc && isValidSyntax; i++) {
        const char* token = argv[i];

        if (strcmp(token, "FROM") == 0) {
            if (!expectComma && columnCount > 0) {
                fprintf(stderr, "Syntax error: unexpected 'FROM' after comma.\n");
                isValidSyntax = 0;
                break;
            }
            foundFromKeyword = 1;
            break;
        }

        if (strcmp(token, ",") == 0) {
            if (!expectComma) {
                fprintf(stderr, "Syntax error: unexpected comma ',' before '%s'.\n",
                    argv[i + 1] ? argv[i + 1] : "end of input");
                isValidSyntax = 0;
                break;
            }
            expectComma = 0; 
            continue;
        }

        if (expectComma) {
            fprintf(stderr, "Syntax error: expected ',' before '%s'.\n", token);
            isValidSyntax = 0;
            break;
        }

        if (columnCount >= capacity) {
            capacity *= 2;
            char** tmp = realloc(selectList, capacity * sizeof(char*));
            if (!tmp) {
                isValidSyntax = 0;
                break;
            }
            selectList = tmp;
        }

        selectList[columnCount] = malloc(strlen(token) + 1);
        if (!selectList[columnCount]) {
            isValidSyntax = 0;
            break;
        }
        strcpy(selectList[columnCount], token);
        columnCount++;

        expectComma = 1; 
    }

    if (isValidSyntax) {
        if (!foundFromKeyword) {
            fprintf(stderr, "Syntax error: missing 'FROM' keyword.\n");
            isValidSyntax = 0;
        }
        else if (!expectComma && columnCount > 0) {
            fprintf(stderr, "Syntax error: dangling comma at end of SELECT list.\n");
            isValidSyntax = 0;
        }
    }

    if (!isValidSyntax) {
        for (int i = 0; i < columnCount; i++)
            free(selectList[i]);
        free(selectList);
        return NULL;
    }

    *listArgs = columnCount;
    return selectList;
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

int extractTableName(const char** argv, int argc, char* outBuffer, size_t bufSize)
{
    int foundFrom = 0;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "FROM") == 0) {
            foundFrom = 1;
            continue;
        }

        if (foundFrom) {
            strncpy(outBuffer, argv[i], bufSize - 1);
            outBuffer[bufSize - 1] = '\0';
            return 1; 
        }
    }

    return 0; 
}

char** extractColumnsToInsert(const char** argv, int argc, int startPos, int* columnsSize)
{
    int currentSize = 0;
    int maxSize = 4;
    char** extractedColumns = malloc(sizeof(char*) * maxSize);
    if (!extractedColumns)
        return NULL;

    int i = startPos;
    for (; i < argc; i++) {
        if (strcmp(argv[i], ")") == 0) {
            break;
        }

        if (strcmp(argv[i], ",") == 0)
            continue;

        if (currentSize >= maxSize) {
            maxSize *= 2;
            extractedColumns = realloc(extractedColumns, sizeof(char*) * maxSize);
            if (!extractedColumns)
                return NULL;
        }

        extractedColumns[currentSize] = _strdup(argv[i]);
        currentSize++;
    }

    if (i >= argc || strcmp(argv[i], ")") != 0) {
        printf("ERROR: missing closing parenthesis ')'\n");
        free(extractedColumns);
        return NULL;
    }

    if (i + 1 >= argc || strcmp(argv[i + 1], "VALUES") != 0) {
        printf("ERROR: expected keyword 'VALUES' after closing parenthesis\n");
        free(extractedColumns);
        return NULL;
    }

    *columnsSize = currentSize;
    return extractedColumns;
}

char*** extractedValuesToInsert(const char** argv, int argc, int startPos, int* valuesSize)
{
    
}
