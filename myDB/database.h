#ifndef _DATABASE_H_
#define _DATABASE_H_
#include "table.h"
typedef struct {

	char name[50];
	Table** tables;
	int tableCount;

}Database;

Database*	createDatabase(const char* name, int createFolder);
void		addTable(Database* db, Table* table);
Table*		findTable(Database* db, const char* tableName);
int			deleteTable(Database* db, const char * tableName);
void		printDatabase(Database* db);

int			isTableExists(Database* db, const char* name);

void		freeDatabase(Database* db);
#endif
