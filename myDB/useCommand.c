#define _CRT_SECURE_NO_WARNINGS
#include "useCommand.h"
#include "logger.h"
#include "commandValidators.h"
#include <stdio.h>
#include <string.h>

void useCommand(AppContext* app, char** argv, int argc)
{
    
    int check = checkUseCommandValidation(app, argc);
    if (check <= 0)
        return;

    const char* dbName = argv[1];
    for (int i = 0; i < app->databasesSize; i++) {
        if (strcmp(dbName, app->databases[i]->name) == 0) {
            app->currentDatabase = app->databases[i];
            printf("Database changed to '%s'.\n", dbName);
            return;
        }
    }

    printf("Unknown database '%s'.\n", dbName);
}
