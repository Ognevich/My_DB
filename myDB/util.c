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

void freeTwoDimArray(void*** array, int rows)
{
    if (!array || !*array) return;

    void** arr = *array;

    for (int i = 0; i < rows; i++) {
        free(arr[i]);
        arr[i] = NULL;
    }

    free(arr);
    *array = NULL;
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
