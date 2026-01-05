#ifndef _COMMAND_VALIDATORS_H_
#define _COMMAND_VALIDATORS_H_
#include "programState.h"

int checkDatabaseConnection(AppContext* app);
int checkTableExists(AppContext* app, const char* name, int ifNotExists);
int checkDatabaseExists(AppContext* app, const char* name, int ifNotExists);
int checkDatabaseNotExists(AppContext* app, const char* name, int ifExists);

int checkCreateTableArguments(const char *** argv, int argc);

int checkUseCommandValidation(AppContext* app, int argc);
int checkUnuseCommandValidation(AppContext* app, int argc);
int checkSelectCommandValidation(AppContext* app, int argc);
int checkSelectCommandArgsValidation(const char ** argv, int argc);
int checkInsertCommandValidation(AppContext* app, const char ** argv,int argc);
int checkInsertColumnValidation(char ** extractedColumns, int ColumnsSize, Table * table );


int isValidArgs(const char ** args, int argc);
int hasForbiddenSymbol(const char* word);
int startsWithNumber(const char* word);


#endif

