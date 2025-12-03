#include "util.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int increaseTwoDimCharArray(char*** array, int size)
{
    char** temp = realloc(*array, size * sizeof(char*));
    if (!temp)
        return 0;

    *array = temp;
    return 1;
}

void freeTwoDimArray(void*** arr, int rows)
{
    if (!arr || !*arr) return;

    void** a = *arr;

    for (int i = 0; i < rows; i++)
        free(a[i]);

    free(a);
    *arr = NULL;
}

void freeThreeDimArray(void**** array, int rows) {
    if (!array || !*array) return;

    char*** arr = *array;

    for (int i = 0; i < rows; i++) {
        if (!arr[i]) continue;

        for (int j = 0; arr[i][j] != NULL; j++) {
            free(arr[i][j]);
        }

        free(arr[i]);
    }

    free(arr);
    *array = NULL;
}

void freeParsedValues(char*** values, int valuesSize) {
    for (int i = 0; i < valuesSize; i++) {
        if (values[i] != NULL) {
            for (int j = 0; values[i][j] != NULL; j++) {
                free(values[i][j]);
            }
            free(values[i]);
        }
    }
    free(values);
}


void printHeader(const char* headerName)
{
    printf("+----------------------------------------+\n");
    printf("| %s                               |\n", headerName);
    printf("+----------------------------------------+\n");

}

FieldType StrToField(char* filedType)
{
    
    if (strcmp(filedType, "INT") == 0) {
        return INT;
    }
    if(strcmp(filedType, "CHAR") == 0) {
        return CHAR;
    }
    if (strcmp(filedType, "FLOAT") == 0) {
        return FLOAT;
    }
    return NONE;
}

const char* sqlErrorToString(SqlError err) {
    switch (err) {
    case SQL_OK: return "No error";
    case SQL_ERR_SYNTAX: return "Syntax error";
    case SQL_ERR_MEMORY: return "Memory allocation error";
    case SQL_ERR_RESERVED_WORD: return "Identifier is a reserved word";
    case SQL_ERR_FORBIDDEN_SYMBOL: return "Forbidden symbol found";
    case SQL_ERR_MISSING_PAREN: return "Missing parenthesis";
    case SQL_ERR_INVALID_ARGUMENT: return "Invalid argument";
    default: return "Unknown error";
    }
}

void printTokens(const char** tokens, int tokensSize)
{
    for (int i = 0; i < tokensSize; i++) {
        printf("%s\n", tokens[i]);
    }
}

void printParsedValues(char*** values, int valuesSize) {
    printf("Parsed %d value groups:\n", valuesSize);
    for (int i = 0; i < valuesSize; i++) {
        printf("Row %d: ", i + 1);
        if (values[i] != NULL) {
            for (int j = 0; values[i][j] != NULL; j++) {
                printf("'%s'", values[i][j]);
                if (values[i][j + 1] != NULL) {
                    printf(", ");
                }
            }
        }
        printf("\n");
    }
}

int defineColumnSize(FieldType type);

int defineColumnSize(FieldType type)
{
    int value;

    switch (type)
    {
    case INT:
        value = sizeof(int);
        break;
    case CHAR:
        value = 50;
        break;
    case FLOAT:
        value = sizeof(float);
        break;
    default:
        break;
    }

    return value;
}
