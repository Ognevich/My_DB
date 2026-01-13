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

