#define _CRT_SECURE_NO_WARNINGS
#ifndef _TABLE_H_
#define _TABLE_H_
#include "config.h"

typedef enum 
{ 
	INT, 
	FLOAT ,
	CHAR,
	NONE
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


Table* createTable(const char* name);
Table* initNewTable(const char* name);

void addColumn(Table* table, const char* name, FieldType type, int size);
int fillTableColumns(Table* table, char*** innerArgs, int innerSize);

void insertRow(Table* table, Field* fields);
void deleteRow(Table* table, int rowIndex);
void updateRow(Table* table, int rowIndex, Field newValues);

void printTable(Table* table);
void freeTable(Table * table);