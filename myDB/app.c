#define _CRT_SECURE_NO_WARNINGS
#include "app.h"
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "database.h"
#include "table.h"
#include "logger.h"
#include "parser.h"
#include "util.h"
#include "File_Utils.h"

void init(AppContext* app)
{
    scanDatabase(app);
}

void runDB(AppContext* app)
{
    char input[BUFFER_SIZE];
    while (app->state != STOP_PROGRAM) {
        
        printCurrentDbState(app);

        if (!readInput(input, sizeof(input)))
            continue;

        int argSize = 0;
        char** argv = tokenize(input, &argSize);
        if (!argv)
            continue;

        //printTokens(argv, argSize);

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

int readInput(char* buffer, int size)
{
    buffer[0] = '\0';

    char line[BUFFER_SIZE];

    while (1) {
        if (!fgets(line, sizeof(line), stdin))
            return 0;

        line[strcspn(line, "\n")] = '\0';

        if (buffer[0] != '\0') {
            strcat(buffer, " ");
        }

        if (strlen(buffer) + strlen(line) + 1 >= size) {
            printf("Input too long.\n");
            return 0;
        }

        strcat(buffer, line);

        int len = strlen(buffer);
        while (len > 0 && isspace((unsigned char)buffer[len - 1])) {
            buffer[--len] = '\0';
        }

        if (len > 0 && buffer[len - 1] == ';') {
            buffer[len - 1] = '\0';
            break;
        }
        printf(" -> ");
    }

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
        if (_stricmp(argv[0], app->commands[i].name) == 0) {
            app->commands[i].handler(app, argv, argc);
            return;
        }
    }
    printf("unknown command\n");
};








