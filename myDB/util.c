#include "util.h"
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
    if (array == NULL || *array == NULL)
        return;

    char** arr = *array;

    for (int i = 0; i < rows; i++) {
        free(arr[i]);
        arr[i] = NULL;
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
