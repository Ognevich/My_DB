#ifndef _PARSER_SELECT_H
#define _PARSER_SELECT_H
#include "parser_keywords.h"
#include "astNode.h"
#include "programState.h"

astNode*	parseSelect(const char ** argv, int argc, SqlError* error);

#endif