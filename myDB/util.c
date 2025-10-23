#include "util.h"
#include <stdio.h>

int countTokensInString(const char* string)
{
    if (string == NULL || *string == '\0')
        return 0;

    int tokens = 1; 
    int i = 0;

    while (string[i] != '\0') {
        if (string[i] == ' ')
            tokens++;
        i++;
    }
    return tokens;
}