#include "parser_where.h"
#include "config.h"
#include <stdio.h>

astNode* parseWhere(const char** argv, int argc, int index, SqlError* error)
{
    *error = SQL_OK;

    int pos = index + 1;

    astNode* left = parseCondition(argv, pos, error);
    if (*error != SQL_OK)
        return NULL;

    pos += 3;

    while (pos < argc)
    {
        if (strcasecmp(argv[pos], "and") != 0 &&
            strcasecmp(argv[pos], "or") != 0)
        {
            break;
        }

        astNodeType type =
            strcasecmp(argv[pos], "and") == 0 ? AST_AND : AST_OR;

        astNode* logic = createAstNode(type);
        logic->left = left;

        astNode* right = parseCondition(argv, pos + 1, error);
        if (*error != SQL_OK)
            return NULL;

        logic->right = right;
        left = logic;

        pos += 4;
    }

    return left;
}