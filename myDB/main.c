#include "table.h"
#include <stdio.h>
#include <string.h>
#include "logger.h"
#include "app.h"
#include "programState.h"

int main(int argc, char * argv[]) {

	AppContext* app = initAppContext();

	runDB(app);
	shutdownDB(app);

}