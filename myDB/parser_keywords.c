#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include "parser_keywords.h"
#include "config.h"

const char* reservedWords[] = {
    "CREATE", "DATABASE", "TABLE", "IF", "NOT", "EXISTS", "SELECT", "INSERT", "UPDATE", "DELETE", "DROP", "WHERE"
};

OperatorMap operators[] = {
    {"<", OP_LT},
    {">", OP_GT},
    {"=", OP_EQ},
    {"<=", OP_LE},
    {">=", OP_GE},
    {"!=", OP_NE},
};

const int operatorsSize = sizeof(operators) / sizeof(operators[0]);
const int reservedWordsCount = sizeof(reservedWords) / sizeof(reservedWords[0]);

int isReservedWord(const char* word)
{
    for (int i = 0; i < reservedWordsCount; i++) {
        if (strcasecmp(reservedWords[i], word) == 0)
            return 1;
    }
    return 0;
}

OP_TYPE parseOperator(const char* op, SqlError* error)
{
    for (int i = 0; i < operatorsSize; i++)
    {
        if (strcmp(op, operators[i].text) == 0)
            return operators[i].type;
    }
    *error = SQL_ERR_SYNTAX;
    return OP_INVALID;
}
