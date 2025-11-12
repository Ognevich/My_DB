#include <string.h>
#include <stdio.h>
#include "insertCommand.h"
#include "commandValidators.h"
#include "parser.h"

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

	const char** extractedColumns = NULL;
	const char*** extractedValues = NULL;
	int columnsSize = 0;
	int valuesSize = 0;

	if (strcmp(argv[3], "(") == 0) {
		extractedColumns = extractColumnsToInsert(argv, argc, 4, &columnsSize);
	}
	else if (strcmp(argv[3],"VALUES") == 0) {
		extractedValues = extractedValuesToInsert(argv, argc, 4,&valuesSize);
	}
	else {
		printf("ERROR: incorrect argument\n");
	}

}
