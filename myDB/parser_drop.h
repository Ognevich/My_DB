#ifndef _PARSER_DROP_H_
#define _PARSER_DROP_H_
#include "parser_keywords.h"
#include "astNode.h"

int ifExistsUsed(const char  ** argv, int argc);
void extractObjName(const char ** argv, int argc, const char ** name, int isExists);

SqlError extractDropTableNames(char *** tableNames, int* size,  const char ** argv, int argc, int isExists);
astNode* parseDropTable(const char ** argv, const argc, SqlError * error);

#endif
