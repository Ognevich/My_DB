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
    const int width = 42; 
    printf("+----------------------------------------+\n");
    printf("| Database                               |\n");
    printf("+----------------------------------------+\n");

    for (int i = 0; i < app->databasesSize; i++) {
        const char* name = app->databases[i]->name;
        int nameLen = strlen(name);

        int spaces = width - 3 - nameLen; 

        printf("| %s", name);
        for (int j = 0; j < spaces; j++)
            putchar(' ');
        printf("|\n");
    }

    printf("+----------------------------------------+\n");
}