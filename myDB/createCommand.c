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
	int ifNotExists = isIfNotExistsUsed(argv, argc);
	char* name = NULL;
	extractName(argv, argc, &name,ifNotExists);


	if (name == NULL) {
		printf("Error: Invalid CREATE syntax\n");
		return;
	}

	if (strcasecmp(objectType, "DATABASE") == 0) {
		if (!createDatabaseCommand(app, name, ifNotExists))
			return;
	}
	else if (strcasecmp(objectType, "TABLE") == 0) {
		processCreateTableCommand(app, argv, argc, name, ifNotExists);
	}
	else {
		logMessage(LOG_ERROR, "Unknown object type: %s", objectType);
	}
}
int createDatabaseCommand(AppContext* app, const char* name, int ifNotExists)
{
	if (checkDatabaseExists(app, name, ifNotExists) <= 0) {
		printf("Database %s already exists\n", name);
		return 0;
	}
	
	Database* db = createDatabase(name, 1);
	if (!db)
		return 0;

	if (!registerDatabase(app, db))
		return;

	return 1;
}

void processCreateTableCommand(AppContext* app, char** argv, int argc, const char* name, int ifNotExists)
{
	if (!isBracketsExists(argv, argc, ifNotExists)) {
		printf("Error: Incorrect brackets type\n");
		return;
	}

	int innerArgs = 0;
	const char*** innerBracketsArgv = NULL;

	SqlError err = extractInnerArgs(argv,argc, &innerBracketsArgv,&innerArgs);
	printError(err);

	if (checkCreateTableArguments(innerBracketsArgv, innerArgs) <= 0) 
		return;
	
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

	saveTableToFile(table, app ,name, innerArgs, innerSize);

	if (!registerTableInDatabase(app, table)) return;

	printf("Table '%s' created successfully.\n", table->name);

}