#define _CRT_SECURE_NO_WARNINGS
#include "createCommand.h"
#include "logger.h"
#include "database.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void createCommand(AppContext* app, char** argv, int argc)
{
	if (argc < 3) {
		logMessage(LOG_ERROR, "Usage: CREATE");
		return;
	}

	if (strcmp(argv[1], "DATABASE") == 0) {
		// FORBID ABILITY CREATE DATABASES WITH SAME NAMES
		printf("CREATE DATABASE command is used\n");
		createDatabaseCommand(app, argv[2]);
	}
}

void createDatabaseCommand(AppContext* app, const char* name)
{
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
