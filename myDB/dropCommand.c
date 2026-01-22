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


	SqlError error = SQL_OK;
	astNode* node = parseDropTable(argv, argc, &error);

	if (error != SQL_OK)
	{
		freeAstNode(node);
		printError(error);
		return 0;
	}

	if (!executeDropTable(app, node))
	{
		freeAstNode(node);
		return 0;
	}
		
	return 1;
}

int executeDropTable(AppContext* app, astNode* node)
{
	if (!node || !node->left)
		return 0; 

	int len = astListLenght(node->left);
	for (int i = 0; i < len; i++)
	{
		astNode* tableNode = astListAt(node->left, i);
		if (!tableNode)
			continue; 

		const char* name = tableNode->column;
		if (!name)
			continue;

		if (isTableExists(app->currentDatabase, name))
		{
			char path[DEFAULT_BUFF_SIZE * 2];
			snprintf(path, sizeof(path), "%s/%s/%s.tbl",
				DB_ROOT, app->currentDatabase->name, name);

			if (!removeFile(path))
			{
				printf("Error: failed to remove file for table %s\n", name);
				return 0;
			}

			if (!deleteTable(app->currentDatabase, name))
				return 0;
		}
		else
		{
			printf("Error: table %s does not exist\n", name); 
			return 0;
		}
	}

	return 1;
}