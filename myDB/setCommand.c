#include "setCommand.h"
#include "parser.h"
#include "util.h"
#include "astNode.h"

int execute_set(astNode* node, const char** argv, const int argc, int * start_pos)
{
	SqlError error = SQL_OK;

	astNode * left_node = parse_set_command(argv, argc, start_pos, &error);
	if (error!=SQL_OK)
	{
		printError(error);
		node->left = left_node;
		return 0;
	}


	return 1;
}
