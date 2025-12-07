#define _CRT_SECURE_NO_WARNINGS
#include "parser_select.h"
#include <stdio.h>
#include "parse_util.h"
#include <string.h>

SqlError extractSelectList(const char** argv, int argc, char*** outList, int* listArgs)
{
    int columnCount = 0;
    int capacity = 4;
    char** selectList = safe_malloc(capacity * sizeof(char*));

    int expectComma = 0;
    int foundFromKeyword = 0;
    int isValidSyntax = 1;
    
    SqlError error;

    for (int i = 1; i < argc && isValidSyntax; i++) {
        const char* token = argv[i];

        if (strcmp(token, "FROM") == 0) {
            if (!expectComma && columnCount > 0) {
                error = SQL_ERR_RESERVED_WORD;
                isValidSyntax = 0;
                break;
            }
            foundFromKeyword = 1;
            break;
        }

        if (strcmp(token, ",") == 0) {
            if (!expectComma) {
                error = SQL_ERR_INVALID_ARGUMENT;
                isValidSyntax = 0;
                break;
            }
            expectComma = 0;
            continue;
        }

        if (expectComma) {
            error = SQL_ERR_INVALID_ARGUMENT;
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
            error = SQL_ERR_RESERVED_WORD;
            isValidSyntax = 0;
        }
        else if (!expectComma && columnCount > 0) {
            error = SQL_ERR_SYNTAX;
            isValidSyntax = 0;
        }
    }

    if (!isValidSyntax) {
        freeCharArr(selectList, columnCount);
        return error;
    }

    *outList = selectList;
    *listArgs = columnCount;
    error = SQL_OK;

    return error;
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