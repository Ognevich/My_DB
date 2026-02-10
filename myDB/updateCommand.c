#include "updateCommand.h"
#include "commandValidators.h"
#include "astNode.h"
#include "setCommand.h"
#include <stdio.h>
#include "config.h"

void updateCommand(AppContext* app, const char** argv, int argc)
{
	if (checkUpdateCommandValidation(app, argv, argc) == 1)
		return;

	astNode* node = createAstNode(AST_UPDATE);

	node->table = argv[1];

	Table* table = findTable(app->currentDatabase, node->table);

	if (!execute_set(node, argv, argc))
	{
		return; 
		freeAstNode(node);
	}
	


}
