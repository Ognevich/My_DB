#ifndef _PARSE_SET_H_
#define _PARSE_SET_H_
#include "parser_keywords.h"
#include "astNode.h"

astNode* parse_set_command(const char ** argv, const int argc,int * start_pos, SqlError * error);

#endif
