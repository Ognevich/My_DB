#ifndef _PARSER_DROP_H_
#define _PARSER_DROP_H_
#include "parser_keywords.h"

int ifExistsUsed(const char  ** argv, int argc);
void extractObjName(const char ** argv, int argc, const char ** name, int isExists);

SqlError extractDropTableNames(char *** tableNames, const char ** argv, int argc);

#endif
