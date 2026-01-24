#define _CRT_SECURE_NO_WARNINGS
#include "parser_select.h"
#include <stdio.h>
#include "parse_util.h"
#include <string.h>
#include "config.h"
#include "util.h"

typedef enum {
    SELECT_EXPECT_COLUMN,
    SELECT_EXPECT_COMMA_OR_FROM,
    SELECT_END,
}extractSelectArgsState;

static SqlError extractSelectList(const char** argv, int argc, char*** outList, int* listArgs)
{
    if (!argv || argc < 2 || !outList || !listArgs)
        return SQL_ERR_INVALID_ARGUMENT;

    int columnCount = 0;
    int capacity = 4;

    char** selectList = safe_malloc(capacity * sizeof(char*));
    if (!selectList) return SQL_ERR_MEMORY;

    extractSelectArgsState state = SELECT_EXPECT_COLUMN;

    SqlError error = SQL_OK;

    for (int i = 1; i < argc; i++) {
        const char* token = argv[i];

        switch (state)
        {
        case SELECT_EXPECT_COLUMN:
            if (strcasecmp(token, "FROM") == 0 ) {
                freeCharArr(selectList, columnCount);
                return SQL_ERR_SYNTAX;
            }
            if (strcmp(token, ",") == 0) {
                freeCharArr(selectList, columnCount);
                return SQL_ERR_INVALID_ARGUMENT;
            }

            if (columnCount >= capacity) {
                capacity *= 2;
                char** tmp = safe_realloc(selectList, capacity * sizeof(char*));
                if (!tmp) {
                    freeCharArr(selectList, columnCount);
                    return SQL_ERR_MEMORY;
                }
                selectList = tmp;
            }

            selectList[columnCount] = safe_malloc(strlen(token) + 1);
            if (!selectList[columnCount]) {
                freeCharArr(selectList, columnCount);
                return SQL_ERR_MEMORY;
            }
            strcpy(selectList[columnCount], token);
            columnCount++;

            state = SELECT_EXPECT_COMMA_OR_FROM;
            break;


        case SELECT_EXPECT_COMMA_OR_FROM:
            if (strcmp(token, ",") == 0) {
                state = SELECT_EXPECT_COLUMN;
                break;
            }

            if (strcmp(token, "FROM") == 0 || strcmp(token, "from") == 0) {
                state = SELECT_END;
                break;
            }

            freeCharArr(selectList, columnCount);
            return SQL_ERR_SYNTAX;


        case SELECT_END:
            i = argc;
            break;
        }
    }

    if (state != SELECT_END) {
        freeCharArr(selectList, columnCount);
        return SQL_ERR_SYNTAX;
    }

    *outList = selectList;
    *listArgs = columnCount;
    return SQL_OK;
}


static int extractTableName(const char** argv, int argc, char* outBuffer, size_t bufSize)
{
    int foundFrom = 0;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "FROM") == 0 || strcmp(argv[i], "from") == 0) {
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

int findFromIndex(const char** argv, int argc)
{
    for (int i = 0; i < argc; i++)
    {
        if (strcasecmp(argv[i], "from") == 0)
            return i;
    }
    return -1;
}

int findWhereIndex(const char** argv, int argc)
{
    int index = findFromIndex(argv, argc);
    if (index == -1)
        return -1;

    for (int i = index + 2; i < argc; i++)
    {
        if (strcasecmp(argv[i], "where") == 0)
        {
            return i;
        }
    }
    return -1;
}

astNode* parseWhere(const char** argv, int argc, int index, SqlError* error)
{
    *error = SQL_OK;
    
    int pos = index + 1;

    astNode* left = parseCondition(argv, pos, error);
    if (*error != SQL_OK)
        return NULL;

    pos += 3;

    if (pos >= argc)
        return left;

    if (strcasecmp(argv[pos], "and") == 0 ||
        strcasecmp(argv[pos], "or") == 0)
    {
        astNodeType type = strcasecmp(argv[pos], "and") == 0 ? AST_AND : AST_OR;

    }

}

astNode* parseSelect(const char** argv, int argc, SqlError* error)
{
    *error = SQL_OK;

    astNode* select = createAstNode(AST_SELECT);
    if (!select) {
        *error = SQL_ERR_MEMORY;
        return NULL;
    }

    int selectArraySize = 0;
    char** selectArray = NULL;

    *error = extractSelectList(argv, argc, &selectArray, &selectArraySize);

    if (*error != SQL_OK) {
        freeTwoDimArray(&selectArray, selectArraySize);
        return select;
    }

    select->left = buildColumnList(selectArray, selectArraySize);

    char tableName[TABLE_NAME_SIZE];
    if (!extractTableName(argv, argc, tableName, TABLE_NAME_SIZE)) {
        *error = SQL_ERR_TABLE_NOT_FOUND;
        freeTwoDimArray(&selectArray, selectArraySize);
        return select;
    }

    select->table = _strdup(tableName);

    int whereIndex = findWhereIndex(argv, argc);
    if (whereIndex != -1)
    {
        select->right = parseWhere(argv, argc, whereIndex, error);
        if (*error != SQL_OK)
        {
            freeTwoDimArray(&selectArray, selectArraySize);
            return select;
        }

    }

    freeTwoDimArray(&selectArray, selectArraySize);
    return select;
}
