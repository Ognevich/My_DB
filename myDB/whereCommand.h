#ifndef _WHERE_COMMAND_H_
#define _WHERE_COMMAND_H_
#include "astNode.h"
#include "table.h"

int evalWhere(astNode * node, Table * table, int row);
int evalCondition(astNode * node, Table * table, int row);

void executeSelectWhere(astNode* node, Table* table, const char** columns, const int count);
#endif
