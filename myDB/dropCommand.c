#include "dropCommand.h"
#include <stdio.h>
#include "parser.h"
#include "commandValidators.h"
#include "File_Utils.h"
#include <string.h>
#include "util.h"


void dropCommand(AppContext* app, char** argv, int argc)
{
	if (argc < 3) {
		printf("Error: insufficient number of parameters\n");
		return;
	}

	if (strcasecmp(argv[1], "DATABASE") == 0)
	{
		if (!dropDatabaseCommand(app, argv, argc))
			return;
	}
	else if (strcasecmp(argv[1], "TABLE") == 0)
	{
		if (!dropTableCommand(app, argv, argc))
			return;
	}
	else
	{
		printf("Unknown object type: %s\n", argv[2]);
	}

}

int dropDatabaseCommand(AppContext* app, const char** argv, int argc)
{
	char* name = NULL;
	int ifExists = ifExistsUsed(argv, argc);

	extractObjName(argv, argc, &name, ifExists);
	if (!name)
	{
		printf("Error: Invalid DROP syntax\n");
		return 0;
	}

	if (!checkDatabaseNotExists(app, name, ifExists))
		return 0;

	char dbPath[DEFAULT_BUFF_SIZE];

	snprintf(dbPath, sizeof(dbPath), "%s/%s", DB_ROOT, name);

	if (!removeDirRecursive(dbPath))
	{
		printf("Error: failed to remove directory\n");
		return 0;
	}

	if (app->currentDatabase && strcmp(app->currentDatabase->name, name) == 0)
		app->currentDatabase = NULL;

	freeDatabaseByName(app, name);
	app->databasesSize--;

	return 1;
}

int dropTableCommand(AppContext* app, const char** argv, int argc)
{
	
	if(!checkDatabaseConnection(app))
		return 0;

	char** tableNames = NULL;
	int tableSize = 0;

	SqlError err = extractDropTableNames(&tableNames, &tableSize ,argv, argc, isExists);

	printError(err);

	for (int i = 0; i < tableSize; i++)
	{
		const char* name = tableNames[i];

		if (isTableExists(app->currentDatabase, name))
		{
			char path[DEFAULT_BUFF_SIZE * 2];
			snprintf(path, sizeof(path), "%s/%s/%s.tbl", DB_ROOT, app->currentDatabase->name, name);

			if (!removeFile(path))
				return 0;

			if (!deleteTable(app->currentDatabase, name))
				return 0;

		}
		else
		{
			printf("Error: table %s does not exists\n", tableNames[i]);
			return 0;
		}
	}
		


	freeCharArr(tableNames, tableSize);
	return 1;
}
