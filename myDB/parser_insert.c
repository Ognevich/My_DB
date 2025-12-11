#include "parser_insert.h"
#include "util.h"
#include <stdio.h>
#include "parse_util.h"
#include <string.h>
#include <stdlib.h>


typedef enum {
    INSERT_COLUMNS_EXPECT_COLUMN,
    INSERT_COLUMNS_EXPECT_COMMA_OR_CLOSE_PAREN,
    INSERT_COLUMNS_END
}extractColumnInsertState;

typedef enum {
    INSERT_VALUES_EXPECT_ROW,     
    INSERT_VALUES_EXPECT_COMMA,
    INSERT_VALUES_END_STATE
} extractValuesState;

typedef enum {
    INSERT_VALUESARGS_EXPECT_VALUE,
    INSERT_VALUESARGS_EXPECT_COMMA,
}extractValuesArgsState;


SqlError extractColumnsToInsert(const char** argv, int argc, int startPos, char*** outColumn, int* columnsSize)
{
    if (!argv || !outColumn || !columnsSize || startPos >= argc)
        return SQL_OK;

    int currentSize = 0;
    int maxSize = 4;

    char** extractedColumns = safe_malloc(sizeof(char*) * maxSize);
    if (!extractedColumns)
        return SQL_ERR_MEMORY;

    extractColumnInsertState state = INSERT_COLUMNS_EXPECT_COLUMN;

    for (int i = startPos; i < argc; i++) {

        switch (state)
        {
        case INSERT_COLUMNS_EXPECT_COLUMN:

            if (!strcmp(argv[i], ",") || !strcmp(argv[i], ")")) {
                freeCharArr(extractedColumns, currentSize);
                return SQL_ERR_INVALID_ARGUMENT;
            }

            if (currentSize >= maxSize) {
                maxSize *= 2;
                char** temp = safe_realloc(extractedColumns, sizeof(char*) * maxSize);
                if (!temp) {
                    freeCharArr(extractedColumns, currentSize);
                    return SQL_ERR_MEMORY;
                }
                extractedColumns = temp;
            }

            extractedColumns[currentSize] = _strdup(argv[i]);
            if (!extractedColumns[currentSize]) {
                freeCharArr(extractedColumns, currentSize);
                return SQL_ERR_MEMORY;
            }

            currentSize++;
            state = INSERT_COLUMNS_EXPECT_COMMA_OR_CLOSE_PAREN;
            break;

        case INSERT_COLUMNS_EXPECT_COMMA_OR_CLOSE_PAREN:

            if (strcmp(argv[i], ",") == 0) {
                state = INSERT_COLUMNS_EXPECT_COLUMN;
                break;
            }

            if (strcmp(argv[i], ")") == 0) {
                state = INSERT_COLUMNS_END;
                i = argc; 
                break;
            }

            freeCharArr(extractedColumns, currentSize);
            return SQL_ERR_INVALID_ARGUMENT;

        case INSERT_COLUMNS_END:
            i = argc;
            break;
        }
    }

    if (state != INSERT_COLUMNS_END) {
        freeCharArr(extractedColumns, currentSize);
        return SQL_ERR_MISSING_PAREN;
    }

    *outColumn = extractedColumns;
    *columnsSize = currentSize;

    return SQL_OK;
}

static int parseValues(const char** argv, int argc, int* index, char*** outValues, int columnCount) {
    int rowSize = 0;
    int rowMaxSize = 10;

    char** row = safe_malloc(sizeof(char*) * rowMaxSize);
    if (!row) return -1;

    extractValuesArgsState state = INSERT_VALUESARGS_EXPECT_VALUE;

    while (*index < argc &&
        strcmp(argv[*index], ")") != 0) 
    {
        switch (state)
        {
        case INSERT_VALUESARGS_EXPECT_VALUE:
            if (strcmp(argv[*index], ",") == 0) {
                printError(SQL_ERR_SYNTAX);
                freeCharArr(row, rowSize);
                return -1;
            }

            if (rowSize >= rowMaxSize - 1) {
                char** tmp = resizeRow(row, &rowMaxSize);
                if (!tmp) {
                    freeCharArr(row, rowSize);
                    return -1;
                }
                row = tmp;
            }

            row[rowSize] = copyString(argv[*index]);
            if (!row[rowSize]) {
                freeCharArr(row, rowSize);
                return -1;
            }

            rowSize++;
            (*index)++;
            state = INSERT_VALUESARGS_EXPECT_COMMA;
            break;

        case INSERT_VALUESARGS_EXPECT_COMMA:
            if (strcmp(argv[*index], ",") != 0) {
                printError(SQL_ERR_SYNTAX);
                freeCharArr(row, rowSize);
                return -1;
            }
            (*index)++;
            state = INSERT_VALUESARGS_EXPECT_VALUE;
            break;
        }
    }

    if (rowSize != columnCount) {
        printError(SQL_ERR_INVALID_ARGUMENT);
        freeCharArr(row, rowSize);
        return -1;
    }

    row[rowSize] = NULL;
    *outValues = row;
    return 0;
}
static int parseRow(const char** argv, int argc, int* index, char *** outRow,int columnCount) {
    if (!expectChar(argv, argc, *index, "(")) return -1;
    (*index)++;
    char** row = NULL;
    parseValues(argv, argc, index, &row,columnCount);
    if (!row) return -1;

    if (!expectChar(argv, argc, *index, ")")) {
        freeCharArr(row, columnCount);
        return -1;
    }
    (*index)++;

    char** finalRow = safe_malloc(sizeof(char*) * (columnCount + 1));
    if (!finalRow) {
        freeCharArr(row, columnCount);
        return -1;
    }

    for (int i = 0; i < columnCount; i++) finalRow[i] = row[i];
    finalRow[columnCount] = NULL;
    free(row);

    *outRow = finalRow;
    return 0;
}

SqlError extractedValuesToInsert(const char** argv, int argc, int startPos,char **** outValues,int* valuesSize, int columnCount)
{
    *valuesSize = 0;
    if (!argv || startPos >= argc)
        return SQL_ERR_INVALID_ARGUMENT;

    int maxSize = 10, currentSize = 0;
    char*** extractedValues = safe_malloc(sizeof(char**) * maxSize);

    int i = startPos;

    extractValuesState state = INSERT_VALUES_EXPECT_ROW;

    while (i < argc) {
        switch (state) {

        case INSERT_VALUES_EXPECT_ROW: {
            char** row = NULL;
            parseRow(argv, argc, &i, &row, columnCount);

            if (!row) {
                freeExtractedValues(extractedValues, currentSize);
                return SQL_OK;
            }

            if (currentSize >= maxSize) {
                maxSize *= 2;
                char*** tmp = safe_realloc(extractedValues, sizeof(char**) * maxSize);
                if (!tmp) {
                    freeExtractedValues(extractedValues, currentSize);
                    for (int j = 0; row[j]; j++)
                        free(row[j]);
                    free(row);
                    return SQL_ERR_MEMORY;
                }
                extractedValues = tmp;
            }

            extractedValues[currentSize++] = row;

            state = INSERT_VALUES_EXPECT_COMMA;
            break;
        }

        case INSERT_VALUES_EXPECT_COMMA:
            if (strcmp(argv[i], ",") == 0) {
                i++;
                state = INSERT_VALUES_EXPECT_ROW;
            }
            else {
                freeExtractedValues(extractedValues, currentSize);
                return SQL_ERR_SYNTAX;
            }
            break;
        }
    }

    if (state == INSERT_VALUES_EXPECT_ROW && currentSize > 0) {
        freeExtractedValues(extractedValues, currentSize);
        return SQL_ERR_SYNTAX;
    }

    *valuesSize = currentSize;
    *outValues = extractedValues;

    return SQL_OK;
}
