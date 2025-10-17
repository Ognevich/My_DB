#define _CRT_SECURE_NO_WARNINGS
#ifndef _DB_ENGINE_H_
#define _DB_ENGINE_H_
#include "table.h"

void saveTableToFile(Table* table, char* filename);

Table* loadTableFromFile(char* filename);



#endif

