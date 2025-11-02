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

void createCommand(AppContext* app, char** argv, int argc)
{
	if (argc < 3) {
		logMessage(LOG_ERROR, "Usage: CREATE DATABASE|TABLE <name> [IF NOT EXISTS]");
		return;
	}

	const char* objectType = argv[1];
	int ifNotExists = isIfNotExistsUsed(argv, argc);
	char* name = extractName(argv, argc, ifNotExists);

	if (!name) {
		logMessage(LOG_ERROR, "Invalid CREATE syntax");
		return;
	}

	if (strcmp(objectType, "DATABASE") == 0) {
		createDatabaseCommand(app, name, ifNotExists);
	}
	else if (strcmp(objectType, "TABLE") == 0) {
		processCreateTableCommand(app, argv, argc, name, ifNotExists);
	}
	else {
		logMessage(LOG_ERROR, "Unknown object type: %s", objectType);
	}
}
void createDatabaseCommand(AppContext* app, const char* name, int ifNotExists)
{
	if (isDatabaseExists(app,name)) {
		if (ifNotExists) {
			return;
		}
		else {
			printf("Error: Database '%s' already exists.\n", name);
			return;
		}
	}

	Database* db = createDatabase(name);

	Database **temp = realloc(app->databases, sizeof(Database*) * (app->databasesSize + 1));

	if (!temp){
		logMessage(LOG_ERROR, "database didn't create");
		free(db);
		return;
	}

	app->databases = temp;
	app->databases[app->databasesSize] = db;
	app->databasesSize++;
	printf("Database %s created\n", db->name);

}

void processCreateTableCommand(AppContext* app, char** argv, int argc, const char* name, int ifNotExists)
{
	if (!isBracketsExists(argv, argc, ifNotExists)) {
		printf("Error: Incorrect brackets type\n");
		return;
	}

	int innerArgs = 0;
	const char*** innerBracketsArgv = extractInnerArgs(argv,argc, &innerArgs);

	for (int i = 0; i < innerArgs; i++) {
		printf("Field %d: ", i);
		for (int j = 0; innerBracketsArgv[i][j] != NULL; j++) {
			printf("%s ", innerBracketsArgv[i][j]);
		}
		printf("\n");
	}

	createTableCommand(app, name, innerBracketsArgv,innerArgs,ifNotExists);

	freeThreeDimArray(&innerBracketsArgv, innerArgs);
}

void createTableCommand(AppContext* app, const char* name, char*** innerArgs, int innerSize, int ifNotExists) {
	if (!checkDatabaseConnection(app)) return;

	int check = checkTableExists(app, name, ifNotExists);
	if (check <= 0) return;

	Table* table = initNewTable(name);
	if (!table) return;

	if (!fillTableColumns(table, innerArgs, innerSize)) {
		free(table);
		return;
	}

	if (!registerTableInDatabase(app, table)) return;

	printf("Table '%s' created successfully.\n", table->name);
}