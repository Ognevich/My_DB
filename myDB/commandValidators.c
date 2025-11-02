#include "commandValidators.h"
#include <stdio.h>

int checkDatabaseConnection(AppContext* app) {
    if (app->currentDatabase == NULL) {
        printf("You can't create table without connecting to a database\n");
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