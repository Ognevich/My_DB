#include "updateCommand.h"
#include "commandValidators.h"
#include "astNode.h"
#include "parser.h"
#include "setCommand.h"
#include <stdio.h>
#include "util.h"
#include "config.h"

void updateCommand(AppContext* app, const char** argv, int argc)
{
	const int START_POS = 3;

	if (!checkUpdateCommandValidation(app, argv, argc))
		return;

	astNode* node = createAstNode(AST_UPDATE);

	node->table = argv[1];

	Table* table = findTable(app->currentDatabase, node->table);
	int start_pos = START_POS;

	if (!execute_set(node, argv, argc,&start_pos))
	{
		freeAstNode(node);
		return; 
	}
	
	printf("where not found\n");
	if (start_pos != START_POS)
	{
		printf("where found\n");
		SqlError error = SQL_OK;
		node->right = parseWhere(argv, argc, start_pos, &error);
		if (error != SQL_OK)
		{
			printError(error);
			freeAstNode(node);
			return;
		}

	}
	freeAstNode(node);
	return;

}
