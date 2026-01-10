#include "astNode.h"
#include "parse_util.h"
#include <stdlib.h>

astNode* createAstNode(astNodeType  type)
{
	astNode* node = safe_malloc(sizeof(astNode*));

	node->type = type;
	node->left = NULL;
	node->right = NULL;
	node->table = NULL;
	node->column = NULL;
	node->value = NULL;
	node->op = 0;

	return node;
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

