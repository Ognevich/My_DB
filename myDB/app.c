#include "app.h"
#include <stdio.h>
#include "database.h"
#include "table.h"
#include "logger.h"

void runDB(AppContext* app)
{
    char input[256];

    while (app->state != STOP_PROGRAM) {
        printf("> ");
        if (!fgets(input, sizeof(input), stdin))
            break; 

        input[strcspn(input, "\n")] = '\0';

        

        if (strcmp(input, "exit") == 0) {
            app->state = STOP_PROGRAM;
            return;
        }




    }

}

void shutdownDB(AppContext* app)
{
	closeLogger();
	freeAppContext(&app);
}
