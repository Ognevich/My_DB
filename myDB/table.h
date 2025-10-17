#define _CRT_SECURE_NO_WARNINGS
#ifndef _TABLE_H_
#define _TABLE_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum 
{ 
	INT, 
	FLOAT ,
	CHAR
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
	char name[50];
	Column* columns;
	int columnCount;
	Row* rows;
	int rowCount;
}Table;



#endif


Table* createTable(const char* name);
void addColumn(Table* table, const char* name, FieldType type, int size);
void insertRow(Table* table, Field* fields);
void printTable(Table* table);
void freeTable(Table * table);