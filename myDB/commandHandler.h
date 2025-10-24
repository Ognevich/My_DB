#ifndef _COMMAND_HANDLER_H
#define _COMMAND_HANDLER_H

typedef struct AppContext AppContext;

typedef struct {
    const char* name;
    void (*handler)(AppContext* app, char** argv, int argc);
} Command;

#endif