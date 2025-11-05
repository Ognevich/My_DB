#include "selectCommand.h"
#include "parser.h"
#include "commandValidators.h"
#include "util.h"
#include <stdio.h>

void selectCommand(AppContext* app, const char** argv, int argc)
{

	if (checkSelectCommandValidation(app, argc) <= 0)
		return;

	int selectArraySize = 0;
	const char** selectArray = extractSelectList(argv, argc, &selectArraySize);
	
	if (checkSelectCommandArgsValidation(selectArray, selectArraySize) <= 0)
		return;


	char tableName[TABLE_NAME_SIZE];
	if (!extractTableName(argv, argc, tableName, TABLE_NAME_SIZE)) {
		printf("Error: Table name not found\n");
		return;
	}

	printf("%s\n", tableName);

	freeTwoDimArray(&selectArray, selectArraySize);

}
