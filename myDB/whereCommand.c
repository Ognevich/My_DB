#include "astNode.h"
#include "table.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "whereCommand.h"


int evalWhere(astNode* node, Table* table, int row)
{
    if (!node)
        return 1;

    switch (node->type)
    {
    case AST_CONDITION:
        return evalCondition(node, table, row);
    case AST_AND:
        return evalWhere(node->left, table, row) && evalWhere(node->right, table, row);
    case AST_OR:
        return evalWhere(node->left, table, row) || evalWhere(node->right, table, row);
    default: return 1;
    }

}

int evalCondition(astNode* node, Table* table, int row)
{
    int col = findTableColumnIndex(table, node->column);
    if (col == -1)
        return 0;

    Field* f = &table->rows[row].fields[col];

    switch (f->type)
    {
    case FIELD_INT:
    {
        int val = atoi(node->value);
        switch (node->opType)
        {
        case OP_EQ: return f->iVal == val;
        case OP_GT: return f->iVal > val;
        case OP_LT: return f->iVal < val;
        case OP_GE: return f->iVal >= val;
        case OP_LE: return f->iVal <= val;
        case OP_NE: return f->iVal != val;
        default: return 0;
        }
    }

    case FIELD_FLOAT:
    {
        float val = atof(node->value);
        switch (node->opType)
        {
        case OP_EQ: return fabs(f->fVal - val) < 1e-6;
        case OP_GT: return f->fVal > val;
        case OP_LT: return f->fVal < val;
        case OP_GE: return f->fVal >= val;
        case OP_LE: return f->fVal <= val;
        case OP_NE: return fabs(f->fVal - val) >= 1e-6;
        default: return 0;
        }
    }

    case FIELD_CHAR:
    {
        switch (node->opType)
        {
        case OP_EQ: return strcmp(node->value, f->sVal) == 0;
        case OP_NE: return strcmp(node->value, f->sVal) != 0;
        default: return 0;
        }
    }

    case FIELD_NONE:
    default:
        return 0;
    }
}

void executeSelectWhere(astNode* node, Table* table, const char** columns, const int count)
{
    for (int i = 0; i < table->rowCount; i++)
    {
        if (!evalWhere(node->right, table, i))
            continue;
        if (!columns)
            printRows(table, i, NULL, 0);
        else
            printRows(table, i, columns, count);
    }
}
