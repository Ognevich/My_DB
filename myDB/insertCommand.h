#ifndef _INSERT_COMMAND_H
#define _INSERT_COMMAND_H


#include "programState.h"
#include "astNode.h"

void	insertCommand(AppContext* app, const char** argv, int argc);
int		insertExecute(AppContext* app, astNode * node, Table * table);

#endif
