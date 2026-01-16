#ifndef _INSERT_COMMAND_H
#define _INSERT_COMMAND_H


#include "programState.h"

void	insertCommand(AppContext* app, const char** argv, int argc);
void	insertExecute(AppContext* app, astNode * node);

#endif
