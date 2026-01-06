#include "parser_keywords.h"
#include "config.h"

const char* reservedWords[] = {
    "CREATE", "DATABASE", "TABLE", "IF", "NOT", "EXISTS", "SELECT", "INSERT", "UPDATE", "DELETE", "DROP"
};

const int reservedWordsCount = sizeof(reservedWords) / sizeof(reservedWords[0]);

int isReservedWord(const char* word)
{
    for (int i = 0; i < reservedWordsCount; i++) {
        if (strcasecmp(reservedWords[i], word) == 0)
            return 1;
    }
    return 0;
}