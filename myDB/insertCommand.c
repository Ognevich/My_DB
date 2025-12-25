#include <string.h>
#include <stdio.h>
#include "insertCommand.h"
#include "util.h"
#include "commandValidators.h"
#include "parser.h"

typedef enum {
    INSERT_STATE_START,
    INSERT_STATE_COLUMNS,
    INSERT_STATE_EXPECT_VALUES,
    INSERT_STATE_VALUES,
    INSERT_STATE_EXECUTE,
    INSERT_STATE_END
} InsertState;

void insertCommand(AppContext* app, const char** argv, int argc)
{
    if (checkInsertCommandValidation(app, argv, argc) <= 0)
        return;

    Table* table = findTable(app->currentDatabase, argv[2]);

    const char** extractedColumns = NULL;
    const parsedValue*** extractedValues = NULL;

    int columnsSize = 0;
    int valuesSize = 0;
    int columnCount = 0;

    int index = 3;
    InsertState state = INSERT_STATE_START;

    while (state != INSERT_STATE_END)
    {
        switch (state)
        {
        case INSERT_STATE_START:
            state = (index < argc && strcmp(argv[index], "(") == 0)
                ? INSERT_STATE_COLUMNS
                : INSERT_STATE_EXPECT_VALUES;
            break;

        case INSERT_STATE_COLUMNS:
        {
            SqlError err = extractColumnsToInsert(
                argv, argc, ++index, &extractedColumns, &columnsSize);
            printError(err);

            if (!checkInsertColumnValidation(extractedColumns, columnsSize, table)) {
                state = INSERT_STATE_END;
                break;
            }

            if (!extractedColumns || columnsSize > table->columnCount) {
                printf("ERROR: wrong columns\n");
                state = INSERT_STATE_END;
                break;
            }

            index = index + columnsSize + (columnsSize - 1) + 1;
            state = INSERT_STATE_EXPECT_VALUES;
            break;
        }

        case INSERT_STATE_EXPECT_VALUES:
            if (index >= argc || strcmp(argv[index], "VALUES") != 0) {
                printf("ERROR: missing VALUES\n");
                state = INSERT_STATE_END;
                break;
            }

            index++;
            state = INSERT_STATE_VALUES;
            break;

        case INSERT_STATE_VALUES:
        {
            columnCount = columnsSize ? columnsSize : table->columnCount;

            SqlError err = extractedValuesToInsert(
                argv, argc, index,
                &extractedValues, &valuesSize, columnCount);
            printError(err);

            if (!extractedValues) {
                state = INSERT_STATE_END;
                break;
            }

            state = INSERT_STATE_EXECUTE;
            break;
        }

        case INSERT_STATE_EXECUTE:
            printInsertValues(extractedValues, valuesSize);

            for (int i = 0; i < valuesSize; i++) {

                Field* fields = safe_malloc(sizeof(Field) * table->columnCount);
                memset(fields, 0, sizeof(Field) * table->columnCount);

                if (columnsSize == 0) {
                    for (int k = 0; k < table->columnCount; k++) {
                        fields[k] = parsedValueToField(extractedValues[i][k]);
                    }
                }
                else {
                    for (int k = 0; k < columnsSize; k++) {
                        int colindex = findTableColumnIndex(table, extractedColumns[k]);
                        if (colindex == -1) {
                            printf("ERROR: column not found\n");
                            break;
                        }
                        fields[colindex] = parsedValueToField(extractedValues[i][k]);
                    }
                }

                insertRow(table, fields);
            }
            state = INSERT_STATE_END;
            break;

        default:
            state = INSERT_STATE_END;
        }
    }

    if (extractedColumns)
        freeTwoDimArray((void***)&extractedColumns, columnsSize);

    if (extractedValues)
        freeParsedValues(extractedValues, valuesSize);
}


