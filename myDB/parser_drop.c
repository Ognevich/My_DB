#define _CRT_SECURE_NO_WARNINGS
#include "parser_drop.h"
#include "config.h"
#include <stdlib.h>
#include "parse_util.h"
#include "commandValidators.h"
#include <string.h>

typedef enum {
    DROP_EXPECT_COMMA_OR_END,
    DROP_EXPECT_VALUE
} dropTableState;

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
    
    if (isExists && argc >= 5)
    {
        *name = argv[4];
    }
    else if (!isExists && argc >= 3)
    {
        *name = argv[2];
    }

    if (*name && (isReservedWord(*name) || hasForbiddenSymbol(*name))) {
        *name = NULL;
    }

}

SqlError extractDropTableNames(char*** tableNames, int * size ,const char** argv, int argc, int isExists)
{
    
    dropTableState state = DROP_EXPECT_VALUE;

    int startPos = 5 ? isExists : 3;

    int maxSize = 2;
    int currentSize = 0;

    const char** extractedTableNames = safe_malloc(sizeof(char*) * maxSize);

    for (int i = startPos; i < argc; i++)
    {
        
        const char* token = argv[i];

        switch (state)
        {
        case DROP_EXPECT_VALUE:

            if (strcmp(token, ',') == 0)
            {
                freeCharArr(extractedTableNames, currentSize);
                return SQL_ERR_SYNTAX;
            }

            if (currentSize >= maxSize)
            {
                maxSize *= 2;

                char** tmp = realloc(extractedTableNames, sizeof(char*) * maxSize);
                if (!tmp)
                {
                    freeCharArr(extractedTableNames, currentSize);
                    return SQL_ERR_MEMORY;
                }
                extractedTableNames = tmp;
            }

            extractedTableNames[currentSize] = malloc(strlen(token) + 1);
            if (!extractedTableNames)
            {
                freeCharArr(extractedTableNames, currentSize);
                return SQL_ERR_MEMORY;
            }

            strcpy(extractedTableNames[currentSize], token);

            currentSize++;

            state = DROP_EXPECT_COMMA_OR_END;
            break;

        case DROP_EXPECT_COMMA_OR_END:
            break;
        }
    }




}
