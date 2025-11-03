#include "selectCommand.h"
#include "parser.h"
#include "commandValidators.h"
#include <stdio.h>

void selectCommand(AppContext* app, const char** argv, int argc)
{

	int check = checkSelectCommandValidation(app, argc);
	if (check <= 0)
		return;

	int selectListSize = 0;
	const char** selectList = extractSelectList(argv, argc, &selectListSize);

	if (!selectList) {
		printf("Error: expected keyword\n");
		return;
	}

	for (int i = 0; i < selectListSize; i++) {

		printf("%s\n", selectList[i]);
	}



}
