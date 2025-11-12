#include <string.h>
#include <stdio.h>
#include "insertCommand.h"
#include "commandValidators.h"
#include "parser.h"

void insertCommand(AppContext* app, const char** argv, int argc)
{
	if (checkInsertCommandValidation(app,argv, argc) <= 0)
		return;

	const char** extractedColumns = NULL;
	const char*** extractedValues = NULL;
	int columnsSize = 0;
	int valuesSize = 0;

	if (strcmp(argv[3], "(") == 0) {
		extractedColumns = extractColumnsToInsert(argv, argc, 4, &columnsSize);
	}

	if (extractedColumns == NULL) {
		return;
	}
	for (int i = 0; i < columnsSize; i++) {
		printf("%s\n", extractedColumns[i]);
	}

	if (!isValidArgs(extractedColumns, columnsSize)) {
		return;
	}

	else if (strcmp(argv[3],"VALUES") == 0) {
		extractedValues = extractedValuesToInsert(argv, argc, 4,&valuesSize);
	}
	else {
		printf("ERROR: incorrect argument\n");
	}

}
