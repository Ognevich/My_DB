#define _CRT_SECURE_NO_WARNINGS
#include "createCommand.h"
#include "commandHandler.h"
#include "logger.h"
#include "database.h"
#include "parser.h"
#include "util.h"
#include "commandValidators.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "File_Utils.h"

void createCommand(AppContext* app, char** argv, int argc)
{
	if (argc < 3) {
		logMessage(LOG_ERROR, "Usage: CREATE DATABASE|TABLE <name> [IF NOT EXISTS]");
		return;
	}

	const char* objectType = argv[1];

	if (strcasecmp(objectType, "DATABASE") == 0) {
		if (!createDatabaseCommand(app, argv, argc))
			return;
	}
	else if (strcasecmp(objectType, "TABLE") == 0) {
		if (!processCreateTableCommand(app, argv, argc))
			return;
	}
	else {
		printf("Unknown object type: %s", objectType);
	}
}
int createDatabaseCommand(AppContext* app, const char** argv, const int argc)
{
	SqlError error = SQL_OK;
	astNode* node = parseCreateDatabase(argv, argc, &error);
	if (error)
	{
		printError(error);
		freeAstNode(node);
		return 0;
	}

	if (checkDatabaseExists(app, node->value, node->ifexists) <= 0) {
		printf("Database %s already exists\n", node->value);
		freeAstNode(node);
		return 0;
	}
	
	Database* db = createDatabase(node->value, 1);
	if (!db)
	{
		freeAstNode(node);
		return 0;
	}

	if (!registerDatabase(app, db))
	{
		freeAstNode(node);
		return 0;
	}

	freeAstNode(node);
	return 1;
}

int processCreateTableCommand(AppContext* app, char** argv, int argc)
{
	SqlError error = SQL_OK;
	astNode* node = parseCreateTable(argv, argc, &error);
	if (error)
	{
		printError(error);
		freeAstNode(node);
		return 0;
	}
	
	if (!createTableCommand(app, node))
	{
		freeAstNode(node);
		return 0;
	}

	freeAstNode(node);
	return 1;
}

int createTableCommand(AppContext* app, astNode * node) {
	if (!checkDatabaseConnection(app)) return 0;

	int check = checkTableExists(app, node->table, node->ifexists);
	if (check <= 0) return 0;

	Table* table = initNewTable(node->table);
	if (!table) return 0;

	if (!fillTableColumns(table, node->left)) {
		free(table);
		return 0;
	}

	saveTableToFile(table, app ,node->table);

	if (!registerTableInDatabase(app, table)) return 0;

	printf("Table '%s' created successfully.\n", table->name);
	return 1;
}