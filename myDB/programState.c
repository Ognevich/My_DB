#include "programState.h"
#include <stdlib.h>
#include "createCommand.h"
#include "useCommand.h"
#include "unuseCommand.h"
#include "showCommand.h"
#include "selectCommand.h"
#include "insertCommand.h"
#include "specialCommnad.h"
#include "logger.h"
#include <stdio.h>
#include <string.h>
#include "File_Utils.h"

AppContext* initAppContext(void)
{
    AppContext* program = calloc(1, sizeof(AppContext));
    if (!program)
        return NULL;

    program->state = START_PROGRAM;

    program->commandCount = 7;

#ifdef SPECIAL_COMMANDS
    program->specialCommandCount = 2;
    initSpecialCommands(program);
#endif 

    initCommands(program);

 /*   readDataFromFile(program);*/

    program->databases = NULL;
    program->databasesSize = 0;
    program->currentDatabase = NULL;

    return program;
}

void initCommands(AppContext* app)
{
    app->commands[0] = (Command){"CREATE", createCommand };
    app->commands[1] = (Command){ "USE", useCommand };
    app->commands[2] = (Command){ "UNUSE", unuseCommand };
    app->commands[3] = (Command){ "SHOW", showCommand };
    app->commands[4] = (Command){ "SELECT", selectCommand };
    app->commands[5] = (Command){ "INSERT", insertCommand };
    app->commands[6] = (Command){ "sp", specialCommand };
}

void initSpecialCommands(AppContext* app)
{
    app->specialCommands[0] = (SpecialCommand){"dut" , dutCommand};
    app->specialCommands[1] = (SpecialCommand){"clear", clearCommand};
}

void freeAppContext(AppContext** program)
{
    if (!program || !*program)
        return;

    AppContext* ctx = *program;

    if (ctx->databases) {
        for (int i = 0; i < ctx->databasesSize; i++) {
            if (ctx->databases[i]) {
                free(ctx->databases[i]);
            }
        }
        free(ctx->databases);
    }
    ctx->currentDatabase = NULL;

    free(ctx);
    *program = NULL;
}
int isDatabaseExists(AppContext* app, const char* name)
{
    for (int i = 0; i < app->databasesSize; i++)
        if (strcmp(app->databases[i]->name, name) == 0)     return 1;
    return 0;
}

int registerDatabase(AppContext* app, Database* db)
{
    Database** temp = realloc(app->databases, sizeof(Database*) * (app->databasesSize + 1));

    if (!temp) {
        logMessage(LOG_ERROR, "database didn't create");
        free(db);
        return 0;
    }

    app->databases = temp;
    app->databases[app->databasesSize] = db;
    app->databasesSize++;
    printf("Database %s created\n", db->name);

    return 1;
}


int registerTableInDatabase(AppContext* app, Table* table) {
    Table** temp = realloc(
        app->currentDatabase->tables,
        sizeof(Table*) * (app->currentDatabase->tableCount + 1)
    );

    if (!temp) {
        logMessage(LOG_ERROR, "Failed to allocate memory for new table");
        free(table);
        return 0;
    }

    app->currentDatabase->tables = temp;
    app->currentDatabase->tables[app->currentDatabase->tableCount] = table;
    app->currentDatabase->tableCount++;
    return 1;
}
