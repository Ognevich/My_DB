#include "selectCommand.h"
#include "parser.h"
#include "commandValidators.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "whereCommand.h"

void selectCommand(AppContext* app, const char** argv, int argc)
{

	if (checkSelectCommandValidation(app, argc) <= 0)
		return;

	SqlError error;

	astNode* node = parseSelect(argv, argc, &error);

	if (error != SQL_OK)
	{
		printError(error);
		freeAstNode(node);
		return;
	}

	if (!validateSelectAst(app, node))
	{
		freeAstNode(node);
		return;
	}

	executeSelect(app, node);
	freeAstNode(node);
}

void executeSelect(AppContext* app, astNode* node)
{
    Table* table = findTable(app->currentDatabase, node->table);
    if (!table)
        return;

    if (node->left &&
        node->left->type == AST_COLUMN &&
        strcmp(node->left->column, "*") == 0)
    {
        if (node->right)
        {
            executeSelectWhere(node, table, NULL, 0);
            return;
        }
        else
        {
            printTable(table);
            return;
        }
    }

    int count = 0;
    for (astNode* c = node->left; c; c = c->right)
        count++;

    char** columns = malloc(sizeof(char*) * count);
    if (!columns)
        return;

    int i = 0;
    for (astNode* c = node->left; c; c = c->right)
        columns[i++] = c->column;

    if (node->right)
    {
        executeSelectWhere(node, table, columns, count);
    }
    else
        printSelectedColumns(table, columns, count);

    free(columns);
}
