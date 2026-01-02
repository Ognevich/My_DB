#ifndef _APP_H_
#define _APP_H_
#include "programState.h"

void	init(AppContext* app);
void	runDB(AppContext * app);
void	shutdownDB(AppContext* app);
void	printCurrentDbState(AppContext * app);
int		readInput(char *buffer, int size);
int		checkExit(AppContext* app, char** argv);

void	handleCommand(AppContext* app, char** argv, int argc);


#endif
