#include "parser_insert.h"
#include <stdio.h>
#include "parse_util.h"
#include <string.h>
#include <stdlib.h>

char** extractColumnsToInsert(const char** argv, int argc, int startPos, int* columnsSize)
{
    int currentSize = 0;
    int maxSize = 4;
    int expectColumn = 1;
    char** extractedColumns = safe_malloc(sizeof(char*) * maxSize);

    for (int i = startPos; i < argc; i++) {
        if (strcmp(argv[i], ")") == 0) {
            if (expectColumn && currentSize > 0) {
                printf("Error: trailing comma before ')'\n");
                break;
            }

            if (i + 1 >= argc || strcmp(argv[i + 1], "VALUES") != 0) {
                printf("ERROR: expected keyword 'VALUES' after closing parenthesis\n");
                break;
            }

            *columnsSize = currentSize;
            return extractedColumns;
        }

        if (strcmp(argv[i], ",") == 0) {
            if (expectColumn) {
                printf("Error: unexpected comma\n");
                break;
            }
            expectColumn = 1;
            continue;
        }

        if (expectColumn) {
            if (currentSize >= maxSize) {
                maxSize *= 2;
                char** temp = safe_realloc(extractedColumns, sizeof(char*) * maxSize);

                extractedColumns = temp;
            }

            extractedColumns[currentSize] = _strdup(argv[i]);
            if (!extractedColumns[currentSize]) {
                printf("Error: strdup failed\n");
                break;
            }
            currentSize++;
            expectColumn = 0;
        }
        else {
            printf("Error: missing comma before '%s'\n", argv[i]);
            break;
        }
    }
    freeCharArr(extractedColumns, currentSize);
    return NULL;
}


char** parseValues(const char** argv, int argc, int* index, int columnCount) {
    int rowSize = 0, rowMaxSize = 10;
    char** row = safe_malloc(sizeof(char*) * rowMaxSize);

    int expectValue = 1;

    while (*index < argc && strcmp(argv[*index], ")") != 0) {

        if (strcmp(argv[*index], ",") == 0) {
            if (expectValue) {
                printf("Syntax error: unexpected ','\n");
                freeCharArr(row, rowSize);
                return NULL;
            }
            expectValue = 1;
            (*index)++;
            continue;
        }

        if (!expectValue) {
            printf("Syntax error: missing ',' between values\n");
            freeCharArr(row, rowSize);
            return NULL;
        }

        if (rowSize >= rowMaxSize) {
            char** tmp = resizeRow(row, &rowMaxSize);
            if (!tmp) {
                freeCharArr(row, rowSize);
                return NULL;
            }
            row = tmp;
        }

        row[rowSize] = copyString(argv[*index]);
        if (!row[rowSize]) {
            freeCharArr(row, rowSize);
            return NULL;
        }

        rowSize++;
        expectValue = 0;
        (*index)++;
    }

    if (rowSize != columnCount) {
        printf("ERROR: too many values in row (expected %d, got %d)\n", columnCount, rowSize);
        freeCharArr(row, rowSize);
        return NULL;
    }

    return row;
}

char** parseRow(const char** argv, int argc, int* index, int columnCount) {
    if (!expectChar(argv, argc, *index, "(")) return NULL;
    (*index)++;

    char** row = parseValues(argv, argc, index, columnCount);
    if (!row) return NULL;

    if (!expectChar(argv, argc, *index, ")")) {
        freeCharArr(row, columnCount);
        return NULL;
    }
    (*index)++;

    char** finalRow = safe_malloc(sizeof(char*) * (columnCount + 1));
    if (!finalRow) {
        freeCharArr(row, columnCount);
        return NULL;
    }

    for (int i = 0; i < columnCount; i++) finalRow[i] = row[i];
    finalRow[columnCount] = NULL;
    free(row);

    return finalRow;
}

// Parse Row End Block

char*** extractedValuesToInsert(const char** argv, int argc, int startPos, int* valuesSize, int columnCount) {
    *valuesSize = 0;
    if (!argv || startPos >= argc) return NULL;

    int maxSize = 10, currentSize = 0;
    char*** extractedValues = safe_malloc(sizeof(char**) * maxSize);

    int i = startPos;
    int expectBlock = 1;

    while (i < argc) {
        if (expectBlock) {
            char** row = parseRow(argv, argc, &i, columnCount);
            if (!row) {
                freeExtractedValues(extractedValues, currentSize);
                return NULL;
            }

            if (currentSize >= maxSize) {
                maxSize *= 2;
                char*** tmp = safe_realloc(extractedValues, sizeof(char**) * maxSize);
                if (!tmp) {
                    freeExtractedValues(extractedValues, currentSize);
                    for (int j = 0; row[j]; j++)
                        free(row[j]);
                    free(row);
                    return NULL;
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
                return NULL;
            }
        }
    }

    if (expectBlock && currentSize > 0) {
        printf("Syntax error: dangling ',' at the end\n");
        freeExtractedValues(extractedValues, currentSize);
        return NULL;
    }

    *valuesSize = currentSize;
    return extractedValues;
}
