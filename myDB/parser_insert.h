#ifndef _PARSER_INSERT_H_
#define _PARSER_INSERT_H_
#include "parser_keywords.h"
#include "astNode.h"
#include "insertCommand.h"

typedef enum {
    INSERT_STATE_START,
    INSERT_STATE_COLUMNS,
    INSERT_STATE_EXPECT_VALUES,
    INSERT_STATE_VALUES,
    INSERT_STATE_EXECUTE,
    INSERT_STATE_END
} InsertState;

SqlError extractedValuesToInsert(
    const char** argv, 
    int          argc, 
    int          startPos, 
    char****     outValues, 
    int*         valuesSize, 
    int          columnCount);

SqlError extractColumnsToInsert(const char** argv, int argc, int startPos, char*** outColumn, int* columnsSize);

SqlError parseInsertValues(astNode * node, const char ** argv, int argc);
SqlError parseInsertColumns(astNode* node, Table* t, const char** argv, int argc, InsertState* state, int* startPos);

int	     isColumninExtractedValues(const char* name, char ** columns,int size);

#endif
