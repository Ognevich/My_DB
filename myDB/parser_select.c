#define _CRT_SECURE_NO_WARNINGS
#include "parser_select.h"
#include <stdio.h>
#include "parse_util.h"
#include <string.h>

char** extractSelectList(const char** argv, int argc, int* listArgs)
{
    int columnCount = 0;
    int capacity = 4;
    char** selectList = safe_malloc(capacity * sizeof(char*));

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
            char** tmp = safe_realloc(selectList, capacity * sizeof(char*));
            if (!tmp) {
                isValidSyntax = 0;
                break;
            }
            selectList = tmp;
        }

        selectList[columnCount] = safe_malloc(strlen(token) + 1);
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
        freeCharArr(selectList, columnCount);
        return NULL;
    }

    *listArgs = columnCount;
    return selectList;
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