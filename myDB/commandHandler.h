#ifndef _COMMAND_HANDLER_H
#define _COMMAND_HANDLER_H
#include "programState.h"

typedef struct{
	const char* name;
	void (*handler)(char** argv, int argc);
}Command;



#endif
