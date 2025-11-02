#ifndef _COMMAND_VALIDATORS_H_
#define _COMMAND_VALIDATORS_H_
#include "programState.h"

//REWORK OTHER FUNCTIONS WITH VALIDATORS

int checkDatabaseConnection(AppContext* app);
int checkTableExists(AppContext* app, const char* name, int ifNotExists);

#endif

