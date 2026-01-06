#ifndef _DROP_COMMAND_H_
#define _DROP_COMMAND_H_
#include "programState.h"

void	dropCommand(AppContext* app, char** argv, int argc);
int		dropDatabaseCommand(AppContext* app, char* name, int ifExists);
int		dropTableCommand(AppContext* app, const char** argv, int argc, int isExists);

#endif
