#include "commandValidators.h"
#include "string.h"
#include "config.h"
#include "logger.h"
#include "parser.h"
#include <stdio.h>
#include <ctype.h>

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

int checkCreateTableArguments(const char*** argv, int argc)
{
    if (argv == NULL) {
        printf("Error: Incorrect args\n");
        return 0;
    }

    for (int i = 0; i < argc; i++)
    {
        if (!isValidArgs(argv[i], COLUMN_ARGUMENTS))
            return 0;
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
        printf("Usage: SELECT <database_name>\n");
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
    if (!isValidArgs(argv, argc))
        return 0;

    return 1;
}

int checkInsertCommandValidation(AppContext* app, int argc)
{
    if (argc < 7) {
        printf("Error: insufficient number of parameters\n");
        return 0;
    }

    if (!checkDatabaseConnection(app))
        return 0;
    return 1;
}

int isValidArgs(const char** args, int argc)
{
    for (int i = 0; i < argc; i++) {
        if (hasForbiddenSymbol(args[i])) {
            printf("Error: argument %s has forbidden symbol\n", args[i]);
            return 0;
        }
        if (startsWithNumber(args[i])) {
            printf("Error: argument %s start with number\n", args[i]);
            return 0;
        }
    }

    

    return 1;
}

int hasForbiddenSymbol(const char* word)
{
    while (*word != '\0') {

        if (strchr(FORBIDEN_SYMBOLS, *word))
            return 1;

        word++;
    }
    return 0;
}

int startsWithNumber(const char* word)
{
    if (word == NULL || *word == '\0') {
        return 0;
    }
    if (isdigit((unsigned char)*word)) {
        return 1;
    }
    return 0;
}


