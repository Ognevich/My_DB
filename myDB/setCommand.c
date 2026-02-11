#include "setCommand.h"
#include "parser.h"
#include "util.h"

int execute_set(astNode* node, const char** argv, const int argc)
{
	int start_pos = 2;
	SqlError error = SQL_OK;


	error = parse_set_command(node, argv, argc);
	if (error!=SQL_OK)
	{
		printError(error);
		return 0;
	}


	return 1;
}
