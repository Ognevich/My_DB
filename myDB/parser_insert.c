#include "parser_insert.h"
#include <stdio.h>
#include "parse_util.h"
#include <string.h>
#include <stdlib.h>

int extractColumnsToInsert( const char** argv, int argc, int startPos, char*** outColumn, int* columnsSize)
{
    if (!argv || !outColumn || !columnsSize || startPos >= argc)
        return -1;

    int currentSize = 0;
    int maxSize = 4;
    int expectColumn = 1;

    char** extractedColumns = safe_malloc(sizeof(char*) * maxSize);
    if (!extractedColumns)
        return -1;

    for (int i = startPos; i < argc; i++) {

        if (strcmp(argv[i], ")") == 0) {

            if (expectColumn && currentSize > 0) {
                printf("Error: trailing comma before ')'\n");
                freeCharArr(extractedColumns, currentSize);
                return -1;
            }

            if (i + 1 >= argc || strcmp(argv[i + 1], "VALUES") != 0) {
                printf("ERROR: expected keyword 'VALUES' after ')'\n");
                freeCharArr(extractedColumns, currentSize);
                return -1;
            }

            *outColumn = extractedColumns;
            *columnsSize = currentSize;
            return 0;
        }

        if (strcmp(argv[i], ",") == 0) {
            if (expectColumn) {
                printf("Error: unexpected comma\n");
                freeCharArr(extractedColumns, currentSize);
                return -1;
            }
            expectColumn = 1;
            continue;
        }

        if (expectColumn) {

            if (currentSize >= maxSize) {
                maxSize *= 2;
                char** temp = safe_realloc(extractedColumns, sizeof(char*) * maxSize);
                if (!temp) {
                    printf("Error: realloc failed\n");
                    freeCharArr(extractedColumns, currentSize);
                    return -1;
                }
                extractedColumns = temp;
            }

            extractedColumns[currentSize] = _strdup(argv[i]);
            if (!extractedColumns[currentSize]) {
                printf("Error: strdup failed\n");
                freeCharArr(extractedColumns, currentSize);
                return -1;
            }

            currentSize++;
            expectColumn = 0;
        }
        else {
            printf("Error: missing comma before '%s'\n", argv[i]);
            freeCharArr(extractedColumns, currentSize);
            return -1;
        }
    }

    printf("Error: missing closing ')'\n");
    freeCharArr(extractedColumns, currentSize);
    return -1;
}

int parseValues(const char** argv, int argc, int* index,char *** outValues ,int columnCount) {
    int rowSize = 0;
    int rowMaxSize = 10;
    char** row = safe_malloc(sizeof(char*) * rowMaxSize);

    int expectValue = 1;

    while (*index < argc && strcmp(argv[*index], ")") != 0) {

        if (strcmp(argv[*index], ",") == 0) {
            if (expectValue) {
                printf("Syntax error: unexpected ','\n");
                freeCharArr(row, rowSize);
                return -1;
            }
            expectValue = 1;
            (*index)++;
            continue;
        }

        if (!expectValue) {
            printf("Syntax error: missing ',' between values\n");
            freeCharArr(row, rowSize);
            return -1;
        }

        if (rowSize >= rowMaxSize) {
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
        expectValue = 0;
        (*index)++;
    }

    if (rowSize != columnCount) {
        printf("ERROR: too many values in row (expected %d, got %d)\n", columnCount, rowSize);
        freeCharArr(row, rowSize);
        return -1;
    }
    row[rowSize] = NULL;
    *outValues = row;

    return 0;
}

int parseRow(const char** argv, int argc, int* index, char *** outRow,int columnCount) {
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

int extractedValuesToInsert(const char** argv, int argc, int startPos,char **** outValues,int* valuesSize, int columnCount)
{
    *valuesSize = 0;
    if (!argv || startPos >= argc) return -1;

    int maxSize = 10, currentSize = 0;
    char*** extractedValues = safe_malloc(sizeof(char**) * maxSize);

    int i = startPos;
    int expectBlock = 1;

    while (i < argc) {
        if (expectBlock) {
            char** row = NULL;
            parseRow(argv, argc, &i, &row,columnCount);
            if (!row) {
                freeExtractedValues(extractedValues, currentSize);
                return -1;
            }

            if (currentSize >= maxSize) {
                maxSize *= 2;
                char*** tmp = safe_realloc(extractedValues, sizeof(char**) * maxSize);
                if (!tmp) {
                    freeExtractedValues(extractedValues, currentSize);
                    for (int j = 0; row[j]; j++)
                        free(row[j]);
                    free(row);
                    return -1;
                }
                extractedValues = tmp;
            }

            extractedValues[currentSize++] = row;
            expectBlock = 0;
        }
        else {
            if (strcmp(argv[i], ",") == 0) {
                expectBlock = 1;
                i++;
            }
            else {
                printf("Syntax error: expected ',' between blocks but found '%s'\n", argv[i]);
                freeExtractedValues(extractedValues, currentSize);
                return -1;
            }
        }
    }

    if (expectBlock && currentSize > 0) {
        printf("Syntax error: dangling ',' at the end\n");
        freeExtractedValues(extractedValues, currentSize);
        return -1;
    }

    *valuesSize = currentSize;
    *outValues = extractedValues;
    return 0;
}
