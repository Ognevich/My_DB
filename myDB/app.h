#ifndef _APP_H_
#define _APP_H_
#include "database.h"
#include "table.h"
#include "logger.h"
#include "programState.h"

void runDB(AppContext * app);
void shutdownDB(AppContext* app);

#endif
