#define _CRT_SECURE_NO_WARNINGS
#ifndef _TABLE_H_
#define _TABLE_H_
#include "config.h"

typedef enum 
{ 
	FIELD_INT, 
	FIELD_FLOAT ,
	FIELD_CHAR,
	FIELD_NONE
} FieldType;

typedef struct {
	char name[50];
	FieldType type;
	int size;
} Column;

typedef struct {
	FieldType type;
	union 
	{
		int iVal;
		float fVal;
		char sVal[50];
	};
}Field;

typedef struct {
	Field* fields;
}Row;

typedef struct {
	char name[TABLE_NAME_SIZE];
	Column* columns;
	int columnCount;
	Row* rows;
	int rowCount;
}Table;



#endif


Table*	createTable(const char* name);
Table*	initNewTable(const char* name);

void	addColumn(Table* table, const char* name, FieldType type, int size);
int		fillTableColumns(Table* table, char*** innerArgs, int innerSize);

void	insertRow(Table* table, Field* fields);
void	deleteRow(Table* table, int rowIndex);
void	updateRow(Table* table, int rowIndex, Field* newValues);

void	printTable(Table* table);

void	printSelectedColumns(Table * table, const char **columns, int columnsCount);
void	freeTable(Table * table);

int		isColumnsExists(const char** selectArray, int selectArraySize, Table* table, int* isAsterisk);
int		isColumnExists(Table* table, const char* column);

int		findTableColumnIndex(Table * table, const char * colName);