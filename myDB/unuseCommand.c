#include "unuseCommand.h"
#include "logger.h"
#include <stdio.h>
#include <string.h>

void unuseCommand(AppContext* app, char** argv, int argc)
{
    if (argc != 1) {
        logMessage(LOG_ERROR, "Usage: UNUSE");
        return;
    }

    if (app->currentDatabase == NULL) {
        printf("No database is currently in use.\n");
        return;
    }

    printf("Stopped using database '%s'.\n", app->currentDatabase->name);
    app->currentDatabase = NULL;
}
