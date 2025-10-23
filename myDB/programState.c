#include "programState.h"
#include <stdlib.h>

AppContext* initAppContext(void)
{
    AppContext* program = malloc(sizeof(AppContext));
    if (!program)
        return NULL;

    program->state = START_PROGRAM;
    return program;
}

void freeAppContext(AppContext** program)
{
    if (!program || !*program)
        return;

    free(*program);
    *program = NULL;

}

