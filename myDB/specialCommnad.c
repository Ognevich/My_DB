#include "specialCommnad.h"
#include "stdio.h"
#include <string.h>

#define commandListSize 5
const char commandCombination[][commandListSize] = { "duti", "abc" };


void specialCommand(AppContext* app, const char** argv, int argc)
{
	if (argc < 2)
	{
		printf("Error: insufficient number of parameters\n");
		return;
	}


	for (int i = 0; i < argc; i++) {
		int counter = 1;
		for (int j = 0; j < commandListSize; j++) {
			counter++;
			if (strcmp(argv[counter], commandCombination[i][j]) == 0) {

			}
		}
	}

}
