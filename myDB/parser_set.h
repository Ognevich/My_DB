#ifndef _PARSE_SET_H_
#define _PARSE_SET_H_
#include "parser_keywords.h"
#include "astNode.h"

SqlError parse_set_command(astNode * node,const char ** argv, const int argc,int start_pos);

#endif
