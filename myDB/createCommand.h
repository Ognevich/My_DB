#ifndef _CREATE_COMMAND_H
#define _CREATE_COMMAND_H
#include "programState.h"
#include "astNode.h"

void	createCommand(AppContext * app, char ** argv, int argc);
int		createDatabaseCommand(AppContext * app, const char ** argv, const int argc);

int		createTableCommand(AppContext* app,astNode * node);
int		processCreateTableCommand(AppContext* app, char** argv, int argc);

#endif
