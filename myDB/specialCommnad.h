#ifndef _DUTI_COMMAND_H_
#define _DUTI_COMMAND_H_
#include "programState.h"

void specialCommand(AppContext* app, const char** argv, int argc);
void handleSpecialCommand(AppContext* app, const char** argv, const char* detokenizeArg, int argc);

void dutCommand(AppContext* app, const char** argv, int argc);
void clearCommand(AppContext* app, const char** argv, int argc);

#endif
