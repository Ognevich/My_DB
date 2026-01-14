#ifndef _PARSER_INSERT_H_
#define _PARSER_INSERT_H_
#include "parser_keywords.h"
#include "astNode.h"

SqlError extractedValuesToInsert(const char** argv, int argc, int startPos, char**** outValues, int* valuesSize, int columnCount);
SqlError extractColumnsToInsert(const char** argv, int argc, int startPos, char*** outColumn, int* columnsSize);

SqlError parseInsertValues(astNode * node, const char ** argv, int argc);
SqlError parseInsertColumns(astNode* node, const char** argv, int argc);

int	     isColumninExtractedValues(const char* name, char ** columns,int size);

#endif
