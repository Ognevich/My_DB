#include "showCommand.h"
#include "logger.h"
#include <stdio.h>
#include <string.h>
#include "util.h"

void showCommand(AppContext* app, char** argv, int argc)
{
	if (argc < 2) {
		logMessage(LOG_ERROR, "Usage: SHOW");
		return;
	}

	if (strcasecmp(argv[1], "DATABASES")==0) {
		showDatabaseCommand(app);
		return;
	}
    if (strcasecmp(argv[1], "TABLES") == 0) {
        showTablesCommand(app);
        return;
    }

}

void showDatabaseCommand(AppContext* app)
{
    const int width = 42; 

    printHeader("Database");

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

void showTablesCommand(AppContext* app)
{
    const int width = 42;

    if (app->currentDatabase == NULL) {
        logMessage(LOG_ERROR, "Usage: SHOW TABLE");
        return;
    }

    printHeader("Tables");

    for (int i = 0; i < app->currentDatabase->tableCount; i++) {
        const char* name = app->currentDatabase->tables[i]->name;
        int nameLen = strlen(name);

        int spaces = width - nameLen - 3;
        
        printf("| %s", name);
        for (int j = 0; j < spaces; j++)
            putchar(' ');
        printf("|\n");
    }
    printf("+----------------------------------------+\n");


}
