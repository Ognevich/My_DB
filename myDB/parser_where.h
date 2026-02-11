#ifndef _PARSER_WHERE_H_
#define _PARSER_WHERE_H_
#include "astNode.h"

astNode* parseWhere(const char** argv, int argc, int index, SqlError* error);


#endif
