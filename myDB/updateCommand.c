#include "updateCommand.h"
#include "commandValidators.h"
#include "astNode.h"
#include "parser.h"
#include <stdio.h>
#include "util.h"
#include "config.h"

void updateCommand(AppContext* app, const char** argv, int argc)
{
	if (checkUpdateCommandValidation(app, argv, argc) == 1)
		return;

	astNode* node = createAstNode(AST_UPDATE);

	node->table = argv[1];

	Table* table = findTable(app->currentDatabase, node->table);
	int start_pos = 2;

	if (!execute_set(node, argv, argc,&start_pos))
	{
		return; 
		freeAstNode(node);
	}
	
	SqlError error = SQL_OK;
	node->right = parseWhere(argv, argc, start_pos, &error);
	if (error != SQL_OK)
	{
		printError(error);
		freeAstNode(node);
		return;
	}


}
