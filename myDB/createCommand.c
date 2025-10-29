#define _CRT_SECURE_NO_WARNINGS
#include "createCommand.h"
#include "logger.h"
#include "database.h"
#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void createCommand(AppContext* app, char** argv, int argc)
{
	if (argc < 3) {
		logMessage(LOG_ERROR, "Usage: CREATE");
		return;
	}

	int ifNotExists = isIfNotExistsUsed(argv, argc);

	if (strcmp(argv[1], "DATABASE") == 0)
		createDatabaseCommand(app, argv[argc-1], ifNotExists);

	if (strcmp(argv[1], "TABLE") == 0) 
		createTableCommand(app, argv[argc-1], ifNotExists);

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

void createTableCommand(AppContext* app, const char* name, int ifNotExists)
{

	if (app->currentDatabase == NULL) {
		printf("You can't create table without connecting to a database\n");
		return;
	}

	if (isTableExists(app->currentDatabase, name)) {
		if (ifNotExists) {
			return;
		}
		else {
			printf("Error: Database '%s' already exists.\n", name);
			return;
		}
	}


	Table* table = createTable(name);
	if (!table) {
		logMessage(LOG_ERROR, "Failed to create table structure");
		return;
	}

	Table** temp = realloc(
		app->currentDatabase->tables,
		sizeof(Table*) * (app->currentDatabase->tableCount + 1)
	);

	if (!temp) {
		logMessage(LOG_ERROR, "Failed to allocate memory for new table");
		free(table);
		return;
	}

	app->currentDatabase->tables = temp;
	app->currentDatabase->tables[app->currentDatabase->tableCount] = table;
	app->currentDatabase->tableCount++;

	printf("Table '%s' created successfully.\n", table->name);
}

