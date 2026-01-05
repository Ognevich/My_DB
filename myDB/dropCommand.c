#include "dropCommand.h"
#include <stdio.h>
#include "parser.h"
#include "commandValidators.h"

void dropCommand(AppContext* app, char** argv, int argc)
{
	if (argc < 3) {
		printf("Error: insufficient number of parameters");
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

}
