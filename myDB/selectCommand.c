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


	for (int i = 0; i < selectArraySize; i++) {

		printf("%s\n", selectArray[i]);
	}

	freeTwoDimArray(&selectArray, selectArraySize);

}
