#define _CRT_SECURE_NO_WARNINGS
#include "parser_create.h"
#include "parse_util.h"
#include "parser_keywords.h"
#include "commandValidators.h"
#include "util.h"
#include <string.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    CREATE_EXPECT_OPEN_BRACKET,
    CREATE_EXPECT_COL_NAME,
    CREATE_EXPECT_COL_TYPE,
    CREATE_EXPECT_COMMA_OR_CLOSE_BRACKET,
    CREATE_END
}extractCreateArgsState;


int isIfNotExistsUsed(char** argv, int argSize)
{
    if (argSize < 3)
        return 0;

    for (int i = 0; i <= argSize - 3; i++)
    {
        if (strcasecmp(argv[i], "IF") == 0 &&
            strcasecmp(argv[i + 1], "NOT") == 0 &&
            strcasecmp(argv[i + 2], "EXISTS") == 0)
        {
            return 1;
        }
    }
    return 0;
}

static void extractName(char** argv, int argc, char** name, int ifNotExists)
{
    *name = NULL;
    const char* src = NULL;

    if (ifNotExists && argc >= 6) {
        src = argv[5];
    }
    else if (!ifNotExists && argc >= 3) {
        src = argv[2];
    }

    if (!src)
        return;

    if (isReservedWord(src) || hasForbiddenSymbol(src))
        return;

    *name = _strdup(src); 
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

static SqlError extractInnerArgs(const char** argv, int argc, char**** outResult, int* innerArgs)
{
    if (!argv || argc <= 0 || !outResult || !innerArgs)
        return SQL_ERR_INVALID_ARGUMENT;

    *outResult = NULL;
    *innerArgs = 0;

    int arraySize = 4;
    int counter = 0;

    char*** result = safe_malloc(arraySize * sizeof(char**));
    if (!result) return SQL_ERR_MEMORY;

    char* currentName = NULL;
    char* currentType = NULL;

    extractCreateArgsState state = CREATE_EXPECT_OPEN_BRACKET;

    for (int i = 0; i < argc; i++) {
        const char* token = argv[i];

        switch (state)
        {
        case CREATE_EXPECT_OPEN_BRACKET:
            if (strcmp(token, "(") == 0) {
                state = CREATE_EXPECT_COL_NAME;
            }
            break;

        case CREATE_EXPECT_COL_NAME:
            if (strcmp(token, ")") == 0) {
                freeInnerArgs(result, counter);
                return SQL_ERR_INVALID_ARGUMENT;
            }

            currentName = _strdup(token);
            if (!currentName) {
                freeInnerArgs(result, counter);
                return SQL_ERR_MEMORY;
            }

            state = CREATE_EXPECT_COL_TYPE;
            break;

        case CREATE_EXPECT_COL_TYPE:
            if (strcmp(token, ")") == 0) {
                free(currentName);
                freeInnerArgs(result, counter);
                return SQL_ERR_INVALID_ARGUMENT;
            }

            currentType = _strdup(token);
            if (!currentType) {
                free(currentName);
                freeInnerArgs(result, counter);
                return SQL_ERR_MEMORY;
            }

            if (!addPair(&result, &counter, &arraySize, currentName, currentType)) {
                free(currentName);
                free(currentType);
                freeInnerArgs(result, counter);
                return SQL_ERR_MEMORY;
            }

            currentName = NULL;
            currentType = NULL;

            state = CREATE_EXPECT_COMMA_OR_CLOSE_BRACKET;
            break;

        case CREATE_EXPECT_COMMA_OR_CLOSE_BRACKET:
            if (strcmp(token, ",") == 0) {
                state = CREATE_EXPECT_COL_NAME;
                break;
            }

            if (strcmp(token, ")") == 0) {
                state = CREATE_END;
                break;
            }

            freeInnerArgs(result, counter);
            return SQL_ERR_SYNTAX;

        case CREATE_END:
            break;
        }
    }

    if (state != CREATE_END) {
        freeInnerArgs(result, counter);
        return SQL_ERR_SYNTAX;
    }

    *innerArgs = counter;
    *outResult = result;

    return SQL_OK;
}

astNode* parseCreateDatabase(const char** argv, int argc, SqlError * error)
{
    astNode* node = createAstNode(AST_CREATE_DATABASE);
    char* name = NULL;

    node->op = isIfNotExistsUsed(argv, argc);
    
    extractName(argv, argc, &name, node->op);
    if (!name)
    {
        *error = SQL_ERR_INCORRECT_DATABASE_NAME;
        return node;
    }

    node->value = _strdup(name);

    return node;
}

astNode* parseCreateTable(char** argv, int argc, SqlError * error)
{
    
    astNode* node = createAstNode(AST_CREATE_TABLE);
    node->op = isIfNotExistsUsed(argv, argc);
    extractName(argv, argc, &node->table, node->op);
    if (!node->table)
    {
        *error = SQL_ERR_INCORRECT_TABLE_NAME;
        return node;
    }

    if (!isBracketsExists(argv, argc, node->op)) {
        *error = SQL_ERR_MISSING_PAREN;
        return node;
    }

    int size = 0;
    const char*** columns = NULL;

    *error = extractInnerArgs(argv, argc, &columns, &size);
    if (*error != SQL_OK)
    {
        freeThreeDimArray(&columns, size);
        return node;
    }

    if (checkCreateTableArguments(columns, size) <= 0)
    {
        *error = SQL_ERR_INVALID_ARGUMENT;
        return node;
    }

    node->left = buildCreateTableColumnsList(columns, size);



    //freeThreeDimArray(&columns, size);
    return node;
}
