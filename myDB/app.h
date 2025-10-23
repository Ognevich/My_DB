#ifndef _APP_H_
#define _APP_H_
#include "programState.h"

void runDB(AppContext * app);
void shutdownDB(AppContext* app);
int readInput(char *buffer, size_t size);
int checkExit(AppContext* app, char** argv);
void handleCommand(AppContext* app, char** argv, int argc);


#endif
