#include <string.h>
#include <stdio.h>
#include "insertCommand.h"
#include "util.h"
#include "commandValidators.h"
#include "parser.h"

void insertCommand(AppContext* app, const char** argv, int argc)
{
    if (checkInsertCommandValidation(app, argv, argc) <= 0)
        return;

    Table* table = findTable(app->currentDatabase, argv[2]);

    const char** extractedColumns = NULL;
    const char*** extractedValues = NULL;

    int columnsSize = 0;
    int valuesSize = 0;

    int success = 1;
    int index = 3;


    if (index < argc && strcmp(argv[index], "(") == 0)
    {
        SqlError err = extractColumnsToInsert(argv, argc, ++index, &extractedColumns ,&columnsSize);
        printError(err);
        if (!extractedColumns)
            success = 0;

        if (success && columnsSize > table->columnCount) {
            printf("ERROR: wrong number of parameters\n");
            success = 0;
        }

        int isAsterics = 0;
        if (success && !isColumnsExists(extractedColumns, columnsSize, table, &isAsterics)) {
            printf("ERROR: columns didn't exists\n");
            success = 0;
        }

        if (success && !isValidArgs(extractedColumns, columnsSize))
            success = 0;

        index = index + columnsSize + (columnsSize-1) + 1; 
    }

    if (success)
    {
        if (index >= argc || strcmp(argv[index], "VALUES") != 0) {
            printf("ERROR: missing VALUES\n");
            success = 0;
        }
    }

    if (success)
        index++;

    int columnCount = 0;

    if (success)
    {

        if (columnsSize)
            columnCount = columnsSize;
        else
            columnCount = table->columnCount;

        SqlError err = extractedValuesToInsert(argv, argc, index, &extractedValues,&valuesSize, columnCount);
        printError(err);
        if (!extractedValues)
            success = 0;
    }

    if (success)
    {
        for (int i = 0; i < columnCount; i++) {

        }
    }

    if (extractedColumns) {
        freeTwoDimArray((void***)&extractedColumns, columnsSize);
        extractedColumns = NULL;
    }
    if (extractedValues) {
        freeParsedValues(extractedValues, valuesSize);
        extractedValues = NULL;
    }

}

