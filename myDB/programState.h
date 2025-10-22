#ifndef _PROGRAM_STATE_H_
#define _PROGRAM_STATE_H_
#include <stdlib.h>

typedef enum {
    START_PROGRAM,
    STOP_PROGRAM
} ProgramState;


typedef struct {
    ProgramState state;
} AppContext;

AppContext* initAppContext();
void freeAppContext(AppContext ** program);


#endif