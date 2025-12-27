#include "util.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

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

void freeParsedValues(char*** values, int valuesSize)
{
    if (!values) return;

    for (int i = 0; i < valuesSize; i++) {
        if (values[i]) {
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
    int maxLenght = 42;
    int nameLenght = strlen(headerName);
    int spaces = maxLenght - 3 - nameLenght;

    printf("+----------------------------------------+\n");
    printf("| %s");
    for (int i = 0; i < spaces; i++) {
        printf(" ");
    }
    printf("|\n");
    printf("+----------------------------------------+\n");

}

void printLine(int size, char symbol)
{
    printf("+");
    for (int i = 0; i < size-2; i++) {
        printf("%c", symbol);
    }
    printf("+");
    printf("\n");
}

FieldType StrToField(char* filedType)
{
    
    if (strcasecmp(filedType, "INT") == 0) {
        return INT;
    }
    if(strcasecmp(filedType, "CHAR") == 0) {
        return CHAR;
    }
    if (strcasecmp(filedType, "FLOAT") == 0) {
        return FLOAT;
    }
    return NONE;
}

Field parsedValueToField(const parsedValue* pv)
{
    Field f;
    memset(&f, 0, sizeof(Field));

    sqlValuesTypeToFieldType(pv->type, &f.type);

    switch (pv->type)
    {
    case SQL_TYPE_INT:
        f.iVal = atoi(pv->raw);
        break;
    case SQL_TYPE_FLOAT:
        f.fVal = atof(pv->raw);
        break;
    case SQL_TYPE_STRING:
        printf("DEBUG raw ptr=%p raw='%s'\n",(void*)pv->raw,pv->raw ? pv->raw : "NULL");
        strncpy(f.sVal, pv->raw, sizeof(f.sVal) - 1);
        f.sVal[sizeof(f.sVal) - 1] = '\0';
        break;

    case SQL_TYPE_NULL:
        f.type = SQL_TYPE_NULL;
        break;
    }

    return f;
}

void sqlValuesTypeToFieldType(const sqlValuesType sType, FieldType * fType)
{
    switch (sType)
    {
    case SQL_TYPE_INT:
        *fType = INT;
        break;
    case SQL_TYPE_FLOAT:
        *fType = FLOAT;
        break;
    case SQL_TYPE_STRING:
        *fType = CHAR;
        break;
    case SQL_TYPE_NULL:
        break;
        *fType = NONE;
    }
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

void printError(SqlError error)
{
    const char * str_error = sqlErrorToString(error);
    if (strcmp(str_error, "No error") == 0)
        return;
    printf("Error: %s\n", str_error);
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

int isInteger(const char* s)
{
    if (!s || !*s) return 0;

    int i = 0;
    if (s[i] == '-' || s[i] == '+') i++;

    if (!isdigit(s[i])) return 0;

    for (; s[i]; i++)
        if (!isdigit(s[i])) return 0;

    return 1;
}

int isFloat(const char* s)
{
    if (!s || !*s) return 0;

    int i = 0;
    int dotCount = 0;
    int digitCount = 0;

    if (s[i] == '+' || s[i] == '-')
        i++;

    for (; s[i]; i++)
    {
        if (isdigit(s[i]))
        {
            digitCount++;
        }
        else if (s[i] == '.')
        {
            dotCount++;
            if (dotCount > 1)
                return 0;
        }
        else
        {
            return 0;
        }
    }

    return (dotCount == 1 && digitCount > 0);
}

int isQuotedString(const char* s)
{
    int len = strlen(s);

    if (len < 2)
        return 0;

    if ((s[0] == '"' && s[len - 1] == '"') || (s[0] == '\'' && s[len - 1] == '\''))
        return 1;

    printf("Error: incorrect backets type\n");
    return 0;
}

int isNULL(const char* s)
{
    if (strcmp(s, "null") == 0 || strcmp(s, "NULL") == 0)
        return 1;
    return 0;
}

void printInsertValues(parsedValue*** value, int size)
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; value[i][j]; j++) {
            if (value[i][j]->raw)
                printf("%s", value[i][j]->raw);
            else
                printf("NULL");

            printf("(%d)", value[i][j]->type); 
            if (value[i][j + 1]) printf(", "); 
        }
        printf("\n"); 
    }
}

int findColumnsLenght(Column* columns,int size)
{
    int lenght = 0;

    for (int i = 0; i < size; i++)
    {
        lenght += strlen(columns[i].name);
    }

    return lenght;
}

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

