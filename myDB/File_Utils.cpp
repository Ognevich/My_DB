#include "File_Utils.h"

int IfFileOpen(FILE* file)
{
	if (!file) {
		perror("Can't open file f");
		return 0;
	}
	return 1;
}
