#ifndef _CREATE_COMMAND_H
#define _CREATE_COMMAND_H
#include "programState.h"


void createCommand(AppContext * app, char ** argv, int argc);
void createDatabaseCommand(AppContext * app, const char * name, int ifNotExists);
void createTableCommand(AppContext* app, const char* name, int ifNotExists);


#endif
