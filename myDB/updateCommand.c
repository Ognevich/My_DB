#include "updateCommand.h"
#include "commandValidators.h"
#include <stdio.h>

void updateCommand(AppContext* app, const char** argv, int argc)
{
	if (checkUpdateCommandValidation(app, argv, argc) == 1)
		return;

}
