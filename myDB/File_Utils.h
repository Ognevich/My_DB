#ifndef _FILE_UTILS_H_
#define _FILE_UTILS_H_
#include <stdio.h>
#include "table.h"

int IfFileOpen(FILE * file);

void writeTableName(FILE* file, Table* table);
void writeColName(FILE* file, Table* table);
void writeColTypes(FILE* file, Table* table);
void writeRows(FILE* file, Table* table);

int readTableName(FILE* file, Table* table);
int readColumns(FILE* file, Table* table);
int readColumnTypes(FILE* file, Table* table);
int readRow(FILE* file, Table* table);

#endif
