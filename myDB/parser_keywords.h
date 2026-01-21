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
    SQL_ERR_TABLE_NOT_FOUND,
    SQL_ERR_INCORRECT_TABLE_NAME,
    SQL_ERR_INCORRECT_DATABASE_NAME,
    SQL_ERR_DEFAULT
} SqlError;

typedef enum {
    SQL_TYPE_INT,
    SQL_TYPE_FLOAT,
    SQL_TYPE_STRING,
    SQL_TYPE_NULL
} sqlValuesType;

typedef struct {
    const char* raw;
    sqlValuesType type;
}parsedValue;


extern const char* reservedWords[];
extern const int reservedWordsCount;

int isReservedWord(const char* word);

#endif

