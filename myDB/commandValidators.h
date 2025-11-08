#ifndef _COMMAND_VALIDATORS_H_
#define _COMMAND_VALIDATORS_H_
#include "programState.h"

int checkDatabaseConnection(AppContext* app);
int checkTableExists(AppContext* app, const char* name, int ifNotExists);
int checkDatabaseExists(AppContext* app, const char* name, int ifNotExists);
int checkUseCommandValidation(AppContext* app, int argc);
int checkUnuseCommandValidation(AppContext* app, int argc);
int checkSelectCommandValidation(AppContext* app, int argc);
int checkSelectCommandArgsValidation(const char ** argv, int argc);
int checkInsertCommandValidation(AppContext* app, int argc);
#endif

