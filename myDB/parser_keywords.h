#ifndef _PARSER_KEYWORDS_H_
#define _PARSER_KEYWORDS_H_

#define BUFFER_SIZE 512

typedef enum {
    SQL_OK,
    SQL_ERR_SYNTAX,
    SQL_ERR_MEMORY,
    SQL_ERR_RESERVED_WORD,
    SQL_ERR_FORBIDDEN_SYMBOL,
    SQL_ERR_MISSING_PAREN,
    SQL_ERR_INVALID_ARGUMENT,
} SqlError;

typedef enum {
    TYPE_INT,
    TYPE_VARCHAR,
    TYPE_BOOLEAN,
    TYPE_UNKNOWN
}SqlType;

extern const char* reservedWords[];
extern const int reservedWordsCount;

int isReservedWord(const char* word);

#endif

