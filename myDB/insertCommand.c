#include <string.h>
#include <stdio.h>
#include "insertCommand.h"
#include "commandValidators.h"

void insertCommand(AppContext* app, const char** argv, int argc)
{
	if (checkInsertCommandValidation(app, argc) <= 0)
		return;

	if (strcmp(argv[1], "INTO") != 0) {
		printf("Error: missed keyword 'INTO'\n");
		return;
	}

	if (checkTableExists(app, argv[2], 1) == 1) {
		printf("Table %s don't exists\n", argv[2]);
		return;
	}

}
