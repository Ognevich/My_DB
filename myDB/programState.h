#ifndef _PROGRAM_STATE_H_
#define _PROGRAM_STATE_H_

#include "commandHandler.h"
#include "database.h"

#define MAX_COMMANDS 10
#define MAX_SPECIAL_COMMANDS 5

typedef enum {
    START_PROGRAM,
    STOP_PROGRAM
} ProgramState;

typedef struct AppContext {
    ProgramState state;

    int commandCount;
    int specialCommandCount;

    Database** databases;
    int databasesSize;
    Database* currentDatabase;

    SpecialCommand specialCommands[MAX_SPECIAL_COMMANDS];
    Command commands[MAX_COMMANDS];
} AppContext;

AppContext* initAppContext();
void        initCommands(AppContext * app);
void        initSpecialCommands(AppContext* app);
void        freeAppContext(AppContext** app);

int         isDatabaseExists(AppContext *app,const char* name);
int         registerDatabase(AppContext *app, Database* db);

int         registerTableInDatabase(AppContext* app, Table* table);

void		freeDatabaseByName(AppContext* app, const char* name);

#endif