#include "selectCommand.h"
#include "parser.h"
#include "commandValidators.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

void executeSelect(AppContext* app, astNode* ast)
{
    Table* table = findTable(app->currentDatabase, ast->table);
    if (!table)
        return;

    if (ast->left &&
        ast->left->type == AST_COLUMN &&
        strcmp(ast->left->column, "*") == 0)
    {
        printTable(table);
        return;
    }

    int count = 0;
    for (astNode* c = ast->left; c; c = c->right)
        count++;

    char** columns = malloc(sizeof(char*) * count);
    if (!columns)
        return;

    int i = 0;
    for (astNode* c = ast->left; c; c = c->right)
        columns[i++] = c->column;

    printSelectedColumns(table, columns, count);
    free(columns);
}
