#ifndef _PARSER_CREATE_H_
#define _PARSER_CREATE_H_
#include "parser_keywords.h"
#include "astNode.h"

int			isIfNotExistsUsed(char** argv, int argSize);
int			isBracketsExists(const char** argv, int argc, int ifNotExists);

astNode*	parseCreateDatabase(const char** argv, int argc, SqlError* error);
astNode*	parseCreateTable(char** argv, int argc, SqlError * error);

#endif

