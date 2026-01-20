#define _CRT_SECURE_NO_WARNINGS
#include "astNode.h"
#include "parse_util.h"
#include <stdlib.h>
#include <string.h>

astNode* createAstNode(astNodeType  type)
{
	astNode* node = calloc(1,sizeof(astNode));

	if (!node)
		return NULL;

	node->type = type;
    node->valueType = SQL_TYPE_NULL;
	return node;
}

astNode* buildColumnList(char** columns, int count)
{
    astNode* head = NULL;
    astNode* current = NULL;

    for (int i = 0; i < count; i++) {
        astNode* node = createAstNode(AST_COLUMN);
        if (!node) {
            freeAstNode(head);
            return NULL;
        }

        node->column = _strdup(columns[i]);

        if (!head)
            head = node;
        else
            current->right = node;

        current = node;
    }

    return head;
}

astNode* buildValuesList(const parsedValue*** values, int rows, int cols)
{
    astNode* rowsHead = NULL;
    astNode* rowsCur = NULL;

    for (int i = 0; i < rows; i++) {

        astNode* row = createAstNode(AST_ROW);
        if (!row) {
            freeAstNode(rowsHead);
            return NULL;
        }

        astNode* valCur = NULL;

        for (int j = 0; j < cols; j++) {
            astNode* v = createAstNode(AST_VALUE);
            if (!v) {
                freeAstNode(rowsHead);
                return NULL;
            }

            v->value = _strdup(values[i][j]->raw);
            v->valueType = values[i][j]->type;

            if (!row->left)
                row->left = v;
            else
                valCur->right = v;

            valCur = v;
        }

        if (!rowsHead)
            rowsHead = row;
        else
            rowsCur->right = row;

        rowsCur = row;
    }

    return rowsHead;
}

astNode* buildCreateTableColumnsList(const char*** columns, int size)
{
    astNode* head = NULL;
    astNode* cur = NULL;

    for (int i = 0; i < size; i++)
    {
        astNode* col = createAstNode(AST_COLUMN);
        astNode* type = createAstNode(AST_TYPE);

        col->column = _strdup(columns[i][0]);
        type = _strdup(columns[i][1]);

        col->left = type;

        if (!head)
            head = col;
        else
            cur->right = col;
        cur = col;

    }
    return head;
}

int astListLenght(astNode* node)
{
    int count = 0;

    while (node)
    {
        count++;
        node = node->right;
    }
    return count;
}

astNode* astListAt(astNode* node, int index)
{
    for (int i = 0; node && i < index; i++)
    {
        node = node->right;
    }
    return node;
}

astNode* astLinkedListAt(astNode* rowsHead, int row, int col)
{
    if (!rowsHead || row < 0 || col < 0)
        return NULL;

    astNode* curRow = rowsHead;

    for (int i = 0; i < row && curRow; i++) {
        curRow = curRow->right;
    }

    if (!curRow)
        return NULL;

    astNode* curVal = curRow->left;

    for (int j = 0; j < col && curVal; j++) {
        curVal = curVal->right;
    }

    return curVal;
}



void freeAstNode(astNode* node)
{
	if (!node) return;

	freeAstNode(node->left);
	freeAstNode(node->right);

	free(node->table);
	free(node->column);
	free(node->value);

	free(node);

}

