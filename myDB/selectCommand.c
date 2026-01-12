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

	int selectArraySize = 0;
	char** selectArray = NULL;
	SqlError error = extractSelectList(argv, argc, &selectArray,&selectArraySize);
	printError(error);

	if (checkSelectCommandArgsValidation(selectArray, selectArraySize) <= 0)
		return;


	char tableName[TABLE_NAME_SIZE];
	if (!extractTableName(argv, argc, tableName, TABLE_NAME_SIZE)) {
		printf("Error: Table argument is empty\n");
		return;
	}

	if (!isTableExists(app->currentDatabase, tableName)) {
		printf("Error: Table %s don't exists\n", tableName);
		return;
	}

	Table* table = findTable(app->currentDatabase, tableName);
	if (!table) {
		printf("Error: Table %s don't exists\n", tableName);
		return;
	}

	int isAsterisk = 0;
	if (!isColumnsExists(selectArray, selectArraySize, table, &isAsterisk)) {
		printf("Error: Wrong select args\n");
		return;
	}

	if (isAsterisk) {
		printTable(table);
	}
	else {
		printSelectedColumns(table, selectArray, selectArraySize);
	}

	freeTwoDimArray(&selectArray, selectArraySize);

}

void executeSelect(AppContext* app, astNode* ast)
{
	Table* table = findTable(app->currentDatabase, ast->table);

	if (!ast->left && ast->type == AST_COLUMN && strcmp(ast->left->column, "*") == 0)
	{
		printTable(table);
		return;
	}

	int count = 0; 

	for (astNode* c = ast->left; c; ast->right)
		count++;

	char** columns = malloc(sizeof(char * ) * count);

	int i = 0;
	
	for (astNode* c = ast->left; c; ast->right)
		columns[i++] = c->column;

	printSelectedColumns(table, columns, count);
	free(columns);
}
