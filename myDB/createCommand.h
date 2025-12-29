#ifndef _CREATE_COMMAND_H
#define _CREATE_COMMAND_H
#include "programState.h"


void createCommand(AppContext * app, char ** argv, int argc);
int createDatabaseCommand(AppContext * app, const char * name, int ifNotExists);

void createTableCommand(AppContext* app, const char* name, char *** innerBracketsArgv,int innerSize,int ifNotExists);
void processCreateTableCommand(AppContext* app, char** argv, int argc, const char* name, int ifNotExists);

#endif
