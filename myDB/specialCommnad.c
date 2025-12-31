#include "specialCommnad.h"
#include "stdio.h"
#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include "File_Utils.h"



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
    if (!db)
        return;

    registerDatabase(app, db);
    

    Table* tb = createTable("t");
    addColumn(tb, "id", FIELD_INT, sizeof(int));
    addColumn(tb, "name", FIELD_CHAR, 50);

    const char* col1[] = { "id", "INT" };
    const char* col2[] = { "name", "TEXT" };

    const char** table[] = { col1, col2 };

    saveTableToFile(tb,app,tb->name, (const char***)table, 2);

    addTable(db, tb);

}

void clearCommand(AppContext* app, const char** argv, int argc)
{
    if (app->databasesSize == 0) {
        printf("nothing to clear\n");
        return;
    }

    for (int i = 0; i < app->databasesSize; i++) {

        char path[256];
        snprintf(path, sizeof(path), "%s/%s", DB_ROOT, app->databases[i]->name);

        if (!removeDirRecursive(path))
        {
            printf("Error: failed to remove directory\n");
        }

    }
    app->currentDatabase = NULL;

    for (int i = 0; i < app->databasesSize; i++)
    {
        free(app->databases[i]);
    }

    app->databasesSize = 0;

}
