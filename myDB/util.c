#include "util.h"
#include <stdlib.h>
#include <stdio.h>

int countTokensInString(const char* string)
{
    if (string == NULL || *string == '\0')
        return 0;

    int tokens = 0;
    int inToken = 0;

    while (*string != '\0') {
        if (*string != ' ' && !inToken) {
            tokens++;
            inToken = 1;
        }
        else if (*string == ' ') {
            inToken = 0;
        }
        string++;
    }

    return tokens;
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
