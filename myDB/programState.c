#include "programState.h"
#include <stdlib.h>
#include "createCommand.h"
#include "useCommand.h"
#include "unuseCommand.h"

AppContext* initAppContext(void)
{
    AppContext* program = calloc(1, sizeof(AppContext));
    if (!program)
        return NULL;

    program->state = START_PROGRAM;
    program->commandCount = 3;
    initCommands(program);
    program->databases = NULL;
    program->databasesSize = 0;
    program->currentDatabase = NULL;

    return program;
}

void initCommands(AppContext* app)
{
    app->commands[0] = (Command){ "CREATE", createCommand };
    app->commands[1] = (Command){ "USE", useCommand };
    app->commands[2] = (Command){ "UNUSE", unuseCommand };
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

    free(ctx->currentDatabase);
    free(ctx);
    *program = NULL;
}