#ifndef _UTIL_H_
#define _UTIL_H_
#include "table.h"
#include "parser_keywords.h"

int			increaseTwoDimCharArray(char*** array, int size);
void		freeTwoDimArray(void*** array, int rows);
void		freeThreeDimArray(void**** array, int rows);
void		freeParsedValues(char*** values, int valuesSize);

void		printHeader(const char* headerName);
void		printLine(int size, char symbol);

int			defineColumnSize(FieldType type);

FieldType	StrToField(char * filedType);
int			parsedValueToField(Field * f, const parsedValue * parsedValue, const FieldType columnType);
void		sqlValuesTypeToFieldType(const sqlValuesType sType, FieldType * fType);

const char* sqlErrorToString(SqlError error);
void		printError(SqlError error);

void		printTokens(const char** tokens, int tokensSize);
void		printParsedValues(char *** values,int valuesSize );

int			isInteger(const char* s);
int			isFloat(const char* s);

int			isQuotedString(const char* s);
int			isNULL(const char* s);

void		printInsertValues(parsedValue*** value, int size);

int			findColumnsLenght(Column* columns, int size);

#endif

