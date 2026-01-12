#ifndef _PARSER_SELECT_H
#define _PARSER_SELECT_H
#include "parser_keywords.h"
#include "astNode.h"
#include "programState.h"

SqlError	extractSelectList(const char** argv, int argc, char *** outList ,int* listArgs);
int			extractTableName(const char** argv, int argc, char* outBuffer, size_t bufSize);

astNode*	parseSelect(const char ** argv, int argc, SqlError* error);

#endif