#ifndef _FILE_UTILS_H_
#define _FILE_UTILS_H_
#include <stdio.h>
#include "table.h"
#include "database.h"

#define DB_ROOT "./data"

int IfFileOpen(FILE * file);

//DATABASES

int createDbDirectory(const char* name);
void writeDatabase(FILE * file, Database * database);

// TABLES
void writeTableName(FILE* file, Table* table);
void writeColName(FILE* file, Table* table);
void writeColTypes(FILE* file, Table* table);
void writeRows(FILE* file, Table* table);

int readTableName(FILE* file, Table* table);
int readColumns(FILE* file, Table* table);
int readColumnTypes(FILE* file, Table* table);
int readRow(FILE* file, Table* table);

#endif
