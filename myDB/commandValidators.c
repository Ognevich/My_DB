#include "commandValidators.h"
#include "logger.h"
#include "parser.h"
#include <stdio.h>

int checkDatabaseConnection(AppContext* app) {
    if (app->currentDatabase == NULL) {
        printf("You can't perform this operation without connecting to a database.\n");
        return 0;
    }
    return 1;
}

int checkTableExists(AppContext* app, const char* name, int ifNotExists) {
    if (isTableExists(app->currentDatabase, name)) {
        if (ifNotExists) {
            return 0; 
        }
        else {
            printf("Error: Table '%s' already exists.\n", name);
            return -1; 
        }
    }
    return 1; 
}

int checkDatabaseExists(AppContext* app, const char* name, int ifNotExists)
{
    if (isDatabaseExists(app, name)) {
        if (ifNotExists) {
            return 0;
        }
        else {
            printf("Error: Database '%s' already exists.\n", name);
            return -1;
        }
    }
    return 1;
}

int checkUseCommandValidation(AppContext* app, int argc)
{
    if (argc != 2) {
        logMessage(LOG_ERROR, "Usage: USE <database_name>");
        return 0;
    }

    if (app->databasesSize <= 0) {
        printf("No databases available.\n");
        return 0;
    }
    return 1;
}

int checkUnuseCommandValidation(AppContext* app, int argc)
{
    if (argc != 1) {
        logMessage(LOG_ERROR, "Usage: UNUSE");
        return 0;
    }

    if (app->currentDatabase == NULL) {
        printf("No database is currently in use.\n");
        return 0;
    }
    return 1;
}

int checkSelectCommandValidation(AppContext* app, int argc)
{
    if (argc < 4) {
        printf("Usage: USE <database_name>\n");
        return 0;
    }

    if (!checkDatabaseConnection(app))
        return 0;
    return 1;
}

int checkSelectCommandArgsValidation(const char** argv, int argc)
{

    if (argc == 0)
    {
        printf("Error: insufficient number of parameters\n");
        return 0;
    }
    if (!argv) {
        printf("Error: expected keyword\n");
        return 0;
    }
    if (isKeyWordInArray(argv, argc)) {
        printf("Error: incorrect use of keyword\n");
        return 0;
    }
    return 1;
}


