#include "app.h"
#include <stdio.h>

void runDB(AppContext* app)
{
    char input[256];

    while (app->state != STOP_PROGRAM) {
        printf("> ");
        if (!fgets(input, sizeof(input), stdin))
            break; 

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0)
            app->state = STOP_PROGRAM;
        else
            printf("Ви ввели: %s\n", input);
    }

}

void shutdownDB(AppContext* app)
{
	closeLogger();
	freeAppContext(&app);
}
