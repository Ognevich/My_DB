#include "parser_set.h"
#include "config.h"
#include <stdio.h>
#include <string.h>

typedef enum {
    SET_EXPECT_VALUE,
    SET_EXPECT_COMMA_OR_WHERE
}setExtractState;

static astNode* parseCondition(const char ** argv,const int argc, int * pos, SqlError * error)
{
    astNode* node = createAstNode(AST_CONDITION);

    if (*pos + 3 < argc)
    {
        *error = SQL_ERR_SYNTAX;
        return node;
    }

    for (int i = 0; i < 3; i++)
    {
        int cur_pos = *pos + i;

        if (strcasecmp(argv[cur_pos], ",") == 0 || strcasecmp(argv[cur_pos], "where") == 0)
        {
            return node;
            *error = SQL_ERR_SYNTAX;
        }

    }

    node->column = argv[*pos];
    if (strcasecmp(argv[*pos + 1], "=") != 0)
    {
        *error = SQL_ERR_SYNTAX;
        return node;
    }
    node->opType = OP_EQ;
    node->value  = argv[*pos + 2];

    *pos += 3;

    return node;

}

SqlError parse_set_command(astNode* node, const char** argv, const int argc, int start_pos)
{
    SqlError error = SQL_OK;
    setExtractState state = SET_EXPECT_VALUE;

    int pos = start_pos;

    while (pos < start_pos)
    {
        switch (state)
        {
        case SET_EXPECT_VALUE:
        {




        }
        case SET_EXPECT_COMMA_OR_WHERE:
        {
            if (strcasecmp(argv[cur_pos], ",") != 0 || strcasecmp(argv[cur_pos], "where" != 0))
                return SQL_ERR_SYNTAX;

            if (strcasecmp(argv[cur_pos], "where") == 0)
            {

            }
        }

    }

    return error;
}
