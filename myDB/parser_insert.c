#include "parser_insert.h"
#include "util.h"
#include <stdio.h>
#include "parse_util.h"
#include <string.h>
#include <stdlib.h>
#include "commandValidators.h"

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

SqlError parseInsertValues(astNode* node, const char** argv, int argc)
{
    const parsedValue*** extractedValues = NULL;
    int valuesSize = 0;




    if (extractedValues)
        freeParsedValues(extractedValues, valuesSize);;
}

SqlError parseInsertColumns(astNode* node, Table * t,const char** argv, int argc, InsertState * state, int * index)
{
    const char** extractedColumns = NULL;
    int columnsSize = 0;

    SqlError err = extractColumnsToInsert(argv, argc, *index, &extractedColumns, &columnsSize);
    
    if (err)
    {
        freeTwoDimArray(&extractedColumns, columnsSize);
        return err;
    }

    if (!checkInsertColumnValidation(extractedColumns, columnsSize, t)) {
        *state = INSERT_STATE_END;
        return SQL_ERR_DEFAULT;
    }

    if (!extractedColumns || columnsSize > t->columnCount) {
        printf("ERROR: wrong columns\n");
        *state = INSERT_STATE_END;
        return SQL_ERR_DEFAULT;
    }



    freeTwoDimArray((void***)&extractedColumns, columnsSize);

    *index = *index + columnsSize + (columnsSize - 1) + 1;
    *state = INSERT_STATE_EXPECT_VALUES;
    return SQL_OK;

}

int isColumninExtractedValues(const char* name, char** columns, int size)
{
    for (int i = 0; i < size; i++) {
        if (strcmp(columns[i], name) == 0) {
            return 1;
        }
    }
    return 0;
}

static parsedValue* parseSingleValue(const char* token)
{
    if (!token) return NULL;

    parsedValue* value = malloc(sizeof(parsedValue));
    if (!value) return NULL;

    if (isInteger(token))
    {
        value->raw = copyString(token);
        value->type = SQL_TYPE_INT;
    }
    else if (isFloat(token))
    {
        value->raw = copyString(token);
        value->type = SQL_TYPE_FLOAT;
    }
    else if (isQuotedString(token))
    {
        int len = strlen(token);
        if (len < 2) {
            printf("Error: too few arguments\n");
            free(value);
            return NULL;
        }
        char* unquoted = safe_malloc(len - 1);
        if (!unquoted) {
            free(value);
            return NULL;
        }
        strncpy(unquoted, token + 1, len - 2);
        unquoted[len - 2] = '\0';
        value->raw = unquoted;
        value->type = SQL_TYPE_STRING;
    }
    else if (isNULL(token)) 
    {
        value->raw = NULL;
        value->type = SQL_TYPE_NULL;
    }
    else {
        free(value);
        return NULL;
    }

    return value;
}

static int parseValues(const char** argv, int argc, int* index, parsedValue*** outValues, int columnCount) {
    int rowSize = 0;
    int rowMaxSize = 10;

    parsedValue** row = safe_malloc(sizeof(parsedValue*) * rowMaxSize);
    if (!row) return -1;

    extractValuesArgsState state = INSERT_VALUESARGS_EXPECT_VALUE;

    while (*index < argc && strcmp(argv[*index], ")") != 0) 
    {
        switch (state)
        {
        case INSERT_VALUESARGS_EXPECT_VALUE:
            if (strcmp(argv[*index], ",") == 0) {
                printError(SQL_ERR_SYNTAX);
                freeParsedArr(row, rowSize); 
                return -1;
            }
            if (rowSize >= rowMaxSize - 1) {
                parsedValue** tmp = resizeParsedArr(row, &rowMaxSize); 
                if (!tmp) {
                    freeParsedArr(row, rowSize);
                    return -1;
                }
                row = tmp;
            }

            parsedValue* val = parseSingleValue(argv[*index]);
            if (!val) {
                freeParsedArr(row, rowSize);
                return -1;
            }
            row[rowSize++] = val;

            (*index)++;
            state = INSERT_VALUESARGS_EXPECT_COMMA;
            break;

        case INSERT_VALUESARGS_EXPECT_COMMA:
            if (strcmp(argv[*index], ",") != 0) {
                printError(SQL_ERR_SYNTAX);
                freeParsedArr(row, rowSize);
                return -1;
            }
            (*index)++;
            state = INSERT_VALUESARGS_EXPECT_VALUE;
            break;
        }
    }

    if (rowSize != columnCount) {
        printError(SQL_ERR_INVALID_ARGUMENT);
        freeParsedArr(row, rowSize);
        return -1;
    }

    row[rowSize] = NULL;
    *outValues = row;
    return 0;
}

static int parseRow(const char** argv, int argc, int* index, parsedValue *** outRow,int columnCount) {
    if (!expectChar(argv, argc, *index, "(")) return -1;
    (*index)++;
    parsedValue** row = NULL;
    parseValues(argv, argc, index, &row,columnCount);
    if (!row) return -1;

    if (!expectChar(argv, argc, *index, ")")) {
        freeParsedArr(row, columnCount);
        return -1;
    }
    (*index)++;

    parsedValue** finalRow = safe_malloc(sizeof(parsedValue*) * (columnCount + 1));
    if (!finalRow) {
        freeParsedArr(row, columnCount);
        return -1;
    }

    for (int i = 0; i < columnCount; i++) finalRow[i] = row[i];
    finalRow[columnCount] = NULL;
    free(row);

    *outRow = finalRow;
    return 0;
}

SqlError extractedValuesToInsert(const char** argv, int argc, int startPos,parsedValue **** outValues,int* valuesSize, int columnCount)
{
    *valuesSize = 0;
    if (!argv || startPos >= argc)
        return SQL_ERR_INVALID_ARGUMENT;

    int maxSize = 10, currentSize = 0;
    parsedValue*** extractedValues = safe_malloc(sizeof(parsedValue**) * maxSize);

    int i = startPos;

    extractValuesState state = INSERT_VALUES_EXPECT_ROW;

    while (i < argc) {
        switch (state) {

        case INSERT_VALUES_EXPECT_ROW: {
            parsedValue** row = NULL;
            parseRow(argv, argc, &i, &row, columnCount);

            if (!row) {
                freeExtractedParsedValues(extractedValues, currentSize);
                return SQL_OK;
            }

            if (currentSize >= maxSize) {
                maxSize *= 2;
                parsedValue*** tmp = safe_realloc(extractedValues, sizeof(parsedValue**) * maxSize);
                if (!tmp) {
                    freeExtractedParsedValues(extractedValues, currentSize);
                    for (int j = 0; row[j]; j++) {
                        free(row[j]->raw);
                        free(row[j]);
                    }
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
                freeExtractedParsedValues(extractedValues, currentSize);
                return SQL_ERR_SYNTAX;
            }
            break;
        }
    }

    if (state == INSERT_VALUES_EXPECT_ROW && currentSize > 0) {
        freeExtractedParsedValues(extractedValues, currentSize);
        return SQL_ERR_SYNTAX;
    }

    *valuesSize = currentSize;
    *outValues = extractedValues;

    return SQL_OK;
}
