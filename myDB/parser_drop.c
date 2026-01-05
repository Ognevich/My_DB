#include "parser_drop.h"
#include "config.h"
#include <stdlib.h>

int ifExistsUsed(const char** argv, int argc)
{
    
    for (int i = 0; i <= argc - 2; i++)
    {
        if (strcasecmp(argv[i], "IF") == 0 &&
            strcasecmp(argv[i + 1], "EXISTS") == 0)
        {
            return 1;
        }
    }
    return 0;
}

void extractObjName(const char** argv, int argc, const char** name, int isExists)
{
    *name = NULL;
    
    if (isExists && argc <= 5)
    {
        *name = argv[4];
    }
    else if (!isExists && argc <= 3)
    {
        *name = argv[2];
    }

}
