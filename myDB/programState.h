#ifndef _PROGRAM_STATE_H_
#define _PROGRAM_STATE_H_

#include "commandHandler.h"
#include "database.h"

#define MAX_COMMANDS 10

typedef enum {
    START_PROGRAM,
    STOP_PROGRAM
} ProgramState;

typedef struct AppContext {
    ProgramState state;
    int commandCount;
    Database** databases;
    int databasesSize;
    Database* currentDatabase;
    Command commands[MAX_COMMANDS];
} AppContext;

AppContext* initAppContext();
void initCommands(AppContext * app);
void freeAppContext(AppContext** app);
int isDatabaseExists(AppContext *app,const char* name);

#endif