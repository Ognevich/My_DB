#include "specialCommnad.h"
#include "stdio.h"
#include "parser.h"
#include <stdlib.h>
#include <string.h>



void specialCommand(AppContext* app, const char** argv, int argc)
{
    if (argc < 2)
    {
        printf("Error: insufficient number of parameters\n");
        return;
    }

    char* argCommand = detokenize(argv, argc);

    handleSpecialCommand(app, argv, argCommand, argc);

    free(argCommand);
}

void handleSpecialCommand(AppContext* app, const char** argv, const char* detokenizeArg, int argc)
{
    for (int i = 0; i < app->specialCommandCount; i++) {
        if (strcmp(detokenizeArg, app->specialCommands[i].name) == 0) {
            app->specialCommands[i].handler(app, argv, argc);
            return;
        }
    }
    printf("unknown command\n");

}

void dutCommand(AppContext* app, const char** argv, int argc)
{
    Database* db = createDatabase("test");
    app->currentDatabase = db;
    Table* tb = createTable("t");
    addColumn(tb, "id", INT, sizeof(int));
    addColumn(tb, "name", CHAR, 50);
    addTable(db, tb);

}
