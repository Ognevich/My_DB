#ifndef _FILE_UTILS_H_
#define _FILE_UTILS_H_
#include <stdio.h>
#include "table.h"
#include "database.h"
#include "programState.h"
#define DB_ROOT "./data"

int		IfFileOpen(FILE * file);

//DATABASES

int		directoryExists(const char* path);
int		fileExists(const char* path);

int		createDbDirectory(const char* name);
void	writeDatabase(FILE * file, Database * database);
		
// TABLES
void	writeTableName(FILE* file, Table * table);
void	writeColName(FILE* file, Table* table);
void	writeColTypes(FILE* file, Table* table);
void	writeRow(FILE * file, Field * fields, int size);
void	writeRows(FILE* file, Table* table);

int		readDataFromFile(AppContext* app);

int		scanDatabase(AppContext * app);
void	readDatabase(AppContext* app, const char * dbpath);

int		readMeta(AppContext * app, const char * metapath);
int		readTable(AppContext* app, const char * tablepath);

int		readTableName(FILE* file, Table* table);
int		readColumns(FILE* file, Table* table);
int		readColumnTypes(FILE* file, Table* table);
int		readRow(FILE* file, Table* table);

int		saveTableToFile(Table* table, AppContext* app, const char* name, const char*** args, int size);
int		appendTableRowsToFile(Field* fields, int size, const char * dbName, const char * tableName);

int		removeDirRecursive(const char* path);

#endif
