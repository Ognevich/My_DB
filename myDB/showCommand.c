#include "showCommand.h"
#include "logger.h"
#include <stdio.h>
#include <string.h>

void showCommand(AppContext* app, char** argv, int argc)
{
	if (argc < 2) {
		logMessage(LOG_ERROR, "Usage: UNUSE");
		return;
	}

	if (strcmp(argv[1], "DATABASES")==0) {
		showDatabaseCommand(app);
		return;
	}

}

void showDatabaseCommand(AppContext* app)
{
	printf("+-------------------+\n");
	printf("| Database |\n");
	printf("+-------------------+\n");
	for (int i = 0; i < app->databasesSize; i++) {
		printf("| %s |\n", app->databases[i]->name);

	}
	printf("+-------------------+\n");
}
