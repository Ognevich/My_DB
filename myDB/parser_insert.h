#ifndef _PARSER_INSERT_H_
#define _PARSER_INSERT_H_
#include "parser_keywords.h"
#include "astNode.h"
#include "insertCommand.h"

SqlError parseInsertValues(astNode * node, const char ** argv, int argc);
SqlError parseInsertColumns(astNode* node, Table* t, const char** argv, int argc, int* startPos);

int	     isColumninExtractedValues(const char* name, char ** columns,int size);

#endif
