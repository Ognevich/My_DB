#define _CRT_SECURE_NO_WARNINGS
#include "app.h"
#include <stdio.h>
#include "database.h"
#include "table.h"
#include "logger.h"
#include "parser.h"
#include "util.h"
#include "string.h"

void runDB(AppContext* app)
{
    char input[256];

    while (app->state != STOP_PROGRAM) {
        
        printCurrentDbState(app);

        if (!readInput(input, sizeof(input)))
            continue;

        int argSize = 0;
        char** argv = split(input, &argSize);
        if (!argv)
            continue;

        if (checkExit(app, argv)) {
            freeTwoDimArray(&argv, argSize);
            break;
        }

        handleCommand(app, argv, argSize);

        freeTwoDimArray(&argv, argSize);
    }
}

void shutdownDB(AppContext* app)
{
	closeLogger();
	freeAppContext(&app);
}

void printCurrentDbState(AppContext* app)
{
    if (app->currentDatabase) {
        printf("%s>", app->currentDatabase->name);
    }
    else{
        printf(">");
    }

}

int readInput(char* buffer, size_t size)
{
    if (!fgets(buffer, size, stdin))
        return 0;

    buffer[strcspn(buffer, "\n")] = '\0';
    return strlen(buffer) > 0;
}

int checkExit(AppContext* app, char** argv)
{
    if (argv[0] && strcmp(argv[0], "exit") == 0) {
        app->state = STOP_PROGRAM;
        return 1;
    }
    return 0;
}

void handleCommand(AppContext* app, char** argv, int argc) 
{
    if (argc == 0) {
        return;
    }

    for (int i = 0; i < app->commandCount; i++) {
        if (strcmp(argv[0], app->commands[i].name) == 0) {
            app->commands[i].handler(app, argv, argc);
            return;
        }
    }
    printf("unknown command\n");
};








