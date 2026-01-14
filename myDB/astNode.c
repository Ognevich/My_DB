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

astNode* buildValuesList(const char** values, int size)
{
    astNode* head = NULL;
    astNode* current = NULL;

    for (int i = 0; i < size; i++)
    {
        astNode* node = createAstNode(AST_VALUE);

        if (!node)
        {
            freeAstNode(node);
            return NULL;
        }

        node->value = _strdup(values[i]);

        if (!head)
            head = node;
        else 
            head->right = node;
        
        current = node;
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

