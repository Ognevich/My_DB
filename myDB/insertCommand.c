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

    Table* table = findTable(app->currentDatabase, argv[2]);
    SqlError insError = SQL_OK;

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
            insError = parseInsertColumns(node,table, argv, argc,&index);

            if (insError != SQL_OK)
            {
                state = INSERT_STATE_END;
                break;
            }

            state = INSERT_STATE_EXPECT_VALUES;
            break;
        }

        case INSERT_STATE_EXPECT_VALUES:
            if (index >= argc || strcasecmp(argv[index], "VALUES") != 0) {
                insError = SQL_ERR_SYNTAX;
                state = INSERT_STATE_END;
                break;
            }

            index++;
            state = INSERT_STATE_VALUES;
            break;

        case INSERT_STATE_VALUES:
        {
            insError = parseInsertValues(node,table ,argv, argc, index);

            if (insError)
            {
                state = INSERT_STATE_END;
                break;
            }

            state = INSERT_STATE_EXECUTE;
            break;
        }
        case INSERT_STATE_EXECUTE:

            if (!insertExecute(app, node, table))
                state = INSERT_STATE_END;

            state = INSERT_STATE_END;
            break;
        case INSERT_STATE_END:

            if (insError != SQL_OK)
                printError(insError);

            freeAstNode(node);
            break;
        default:
            state = INSERT_STATE_END;
        }
    }
}

int insertExecute(AppContext* app, astNode* node, Table* table)
{

    int columnsSize = astListLenght(node->left);
    int valuesSize = astListLenght(node->right);

    for (int i = 0; i < valuesSize; i++) {

        Field* fields = safe_malloc(sizeof(Field) * table->columnCount);
        memset(fields, 0, sizeof(Field) * table->columnCount);

        if (columnsSize) {
            for (int k = 0; k < table->columnCount; k++) {
                FieldType columnType = table->columns[k].type;

                if (!parsedValueToField(&fields[k], astLinkedListAt(node->right,i,k), columnType))
                {
                    printf("Error: invalid data types\n");
                    return 0;
                }
            }
        }
        else {
            for (int k = 0; k < columnsSize; k++) {
                int colindex = findTableColumnIndex(table, astListAt(node->left,k));
                if (colindex == -1) {
                    printf("ERROR: column not found\n");
                    return 0;
                }
                FieldType columnType = table->columns[colindex].type;

                if (!parsedValueToField(&fields[colindex], astLinkedListAt(node->right, i, k), columnType))
                {
                    printf("Error: invalid data types\n");
                    return 0;
                }
            }
        }

        insertRow(table, fields);

        if (!appendTableRowsToFile(fields, table->columnCount, app->currentDatabase->name, table->name))
        {
            printf("Error: file not found\n");
            return 0;
        }

        free(fields);
        return 1;
    }
}


