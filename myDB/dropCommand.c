#include "dropCommand.h"
#include <stdio.h>
#include "parser.h"
#include "commandValidators.h"
#include "File_Utils.h"
#include <string.h>

void dropCommand(AppContext* app, char** argv, int argc)
{
	if (argc < 3) {
		printf("Error: insufficient number of parameters\n");
		return;
	}

	int ifExists = ifExistsUsed(argv, argc);
	char* objectType = argv[2];
	char* name = NULL;



	extractObjName(argv, argc, &name, ifExists);

	if (!name)
	{
		printf("Error: Invalid DROP syntax\n");
		return;
	}

	if (strcasecmp(argv[1], "DATABASE") == 0)
	{
		if (!dropDatabaseCommand(app, name, ifExists))
			return;
	}
	else if (strcasecmp(argv[1], "TABLE") == 0)
	{
		if (!dropTableCommand(app, argv, argc, ifExists))
			return;
	}
	else
	{
		printf("Unknown object type: %s\n", objectType);
	}

}

int dropDatabaseCommand(AppContext* app, char* name, int ifExists)
{

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

int dropTableCommand(AppContext* app, const char** argv, int argc, int isExists)
{
	
	char** tableNames = NULL;
	int tableSize = 0;

	SqlError err = extractDropTableNames(&tableNames, &tableSize ,argv, argc, isExists);


}
