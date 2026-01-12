#ifndef _SELECT_COMMAND_H_
#define _SELECT_COMMAND_H_
#include "programState.h"
#include "astNode.h"

void	selectCommand(AppContext * app, const char ** argv, int argc);
void	executeSelect(AppContext* app, astNode* ast);

#endif
