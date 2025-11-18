#include <string.h>
#include <stdio.h>
#include "insertCommand.h"
#include "util.h"
#include "commandValidators.h"
#include "parser.h"

void insertCommand(AppContext* app, const char** argv, int argc)
{
	if (checkInsertCommandValidation(app,argv, argc) <= 0)
		return;

	Table* table = findTable(app->currentDatabase, argv[2]);

	const char** extractedColumns = NULL;
	const char*** extractedValues = NULL;
	int columnsSize = 0;
	int valuesSize = 0;

	if (strcmp(argv[3], "(") == 0) {
		extractedColumns = extractColumnsToInsert(argv, argc, 4, &columnsSize);
		if (extractedColumns == NULL) {
			return;
		}

		int isAsterics = 0;
		if (isColumnsExists(extractedColumns, columnsSize, table, &isAsterics))
		{
			printf("ERROR: Columns didn't exists\n");
			return;
		}

		if (!isValidArgs(extractedColumns, columnsSize)) {
			return;
		}

		

	}
	else if (strcmp(argv[3],"VALUES") == 0) {
		extractedValues = extractedValuesToInsert(argv, argc, 4,&valuesSize);

		if (!extractedValues)
			return;
	}
	else {
		printf("ERROR: incorrect argument\n");
	}

	freeTwoDimArray(extractedColumns, columnsSize);
	freeParsedValues(extractedValues, valuesSize);
}
