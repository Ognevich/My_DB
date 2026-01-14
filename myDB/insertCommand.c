#include <string.h>
#include <stdio.h>
#include "insertCommand.h"
#include "util.h"
#include "commandValidators.h"
#include "parser.h"
#include <stdlib.h>
#include "File_Utils.h"
#include "astNode.h"

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

    astNode* node = createAstNode(AST_INSERT);
    node->table = argv[2];
    if (!node)
        return;

    Table* table = findTable(app->currentDatabase, argv[2]);


    InsertState state = INSERT_STATE_START;
    int index = 3;

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
            index++;
            SqlError err = parseInsertColumns(node,table, argv, argc, &state, &index);

            if (err != SQL_OK)
            {
                printError(err);
                state = INSERT_STATE_END;
            }

            state = INSERT_STATE_EXPECT_VALUES;
            break;
        }

        case INSERT_STATE_EXPECT_VALUES:
            if (index >= argc || strcasecmp(argv[index], "VALUES") != 0) {
                printf("ERROR: missing VALUES\n");
                state = INSERT_STATE_END;
                break;
            }

            index++;
            state = INSERT_STATE_VALUES;
            break;

        case INSERT_STATE_VALUES:
        {

            SqlError err = parseInsertValues();

            int columnCount = columnsSize ? columnsSize : table->columnCount;

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
                        FieldType columnType = table->columns[k].type;
                        
                        if (!parsedValueToField(&fields[k], extractedValues[i][k], columnType)) 
                        {
                            printf("Error: invalid data types\n");
                            return;
                        }
                    }
                }
                else {
                    for (int k = 0; k < columnsSize; k++) {
                        int colindex = findTableColumnIndex(table, extractedColumns[k]);
                        if (colindex == -1) {
                            printf("ERROR: column not found\n");
                            break;
                        }
                        FieldType columnType = table->columns[colindex].type;

                        if (!parsedValueToField(&fields[colindex], extractedValues[i][k], columnType))
                        {
                            printf("Error: invalid data types\n");
                            return;
                        }
                    }
                }

                insertRow(table, fields);
                
                if (!appendTableRowsToFile(fields, table->columnCount, app->currentDatabase->name, table->name))
                {
                    printf("Error: file not found\n");
                    return;
                }

                free(fields);

            }
            state = INSERT_STATE_END;
            break;

        default:
            state = INSERT_STATE_END;
        }
    }
}


