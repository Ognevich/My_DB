#ifndef _PROGRAM_STATE_H_
#define _PROGRAM_STATE_H_
#include "commandHandler.h"

#define MAX_COMMANDS 10

typedef enum {
    START_PROGRAM,
    STOP_PROGRAM
} ProgramState;

typedef struct {
    ProgramState state;
    int commandCount;
    Command commands[MAX_COMMANDS];
} AppContext;



AppContext* initAppContext();
void freeAppContext(AppContext ** program);


#endif