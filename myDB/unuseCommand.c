#include "unuseCommand.h"
#include "logger.h"
#include "commandValidators.h"
#include <stdio.h>
#include <string.h>

void unuseCommand(AppContext* app, char** argv, int argc)
{

    int check = checkUnuseCommandValidation(app, argc);

    if (argc <= 0)
        return;

    printf("Stopped using database '%s'.\n", app->currentDatabase->name);
    app->currentDatabase = NULL;
}
