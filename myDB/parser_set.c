#include "parser_set.h"
#include "config.h"
#include <stdio.h>
#include <string.h>
#include "util.h"

typedef enum {
    SET_EXPECT_VALUE,
    SET_EXPECT_COMMA_OR_WHERE
}setExtractState;




static astNode* parseCondition(const char** argv,
    const int argc,
    int* pos,
    SqlError* error)
{
    astNode* node = createAstNode(AST_CONDITION);

    if (*pos + 2 >= argc)
    {
        *error = SQL_ERR_SYNTAX;
        freeAstNode(node);
        return NULL;
    }

    for (int i = 0; i < 3; i++)
    {
        int cur_pos = *pos + i;

        if (strcasecmp(argv[cur_pos], ",") == 0 ||
            strcasecmp(argv[cur_pos], "where") == 0)
        {
            *error = SQL_ERR_SYNTAX;
            freeAstNode(node);
            return NULL;
        }
    }

    node->column = argv[*pos];

    if (strcasecmp(argv[*pos + 1], "=") != 0)
    {
        *error = SQL_ERR_SYNTAX;
        freeAstNode(node);
        return NULL;
    }

    node->opType = OP_EQ;
    node->value = argv[*pos + 2];

    *pos += 3;

    return node;
}

astNode* parse_set_command(const char** argv, const int argc, int * start_pos, SqlError* error)
{
    setExtractState state = SET_EXPECT_VALUE;

    int pos = *start_pos;

    astNode* node = createAstNode(AST_SET);
    astNode* current = NULL;

    while (pos < argc)
    {
        switch (state)
        {
        case SET_EXPECT_VALUE:
        {
            astNode* left = parseCondition(argv, argc, &pos, error);
            if (*error != SQL_OK)
            {
                freeAstNode(node);
                return NULL;
            }

            if (node->left == NULL)
                node->left = left;
            else
                current->left = left;

            current = left;

            state = SET_EXPECT_COMMA_OR_WHERE;
            break;
        }

        case SET_EXPECT_COMMA_OR_WHERE:
        {
            if (strcasecmp(argv[pos], ",") == 0)
            {
                pos++;
                state = SET_EXPECT_VALUE;
            }
            else if (strcasecmp(argv[pos], "where") == 0)
            {
                *start_pos = pos;
                return node;
            }
            else
            {
                *error = SQL_ERR_SYNTAX;
                return node;
            }
            break;
        }
        }
    }
    return node;
}
