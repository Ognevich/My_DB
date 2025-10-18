#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "db_engine.h"
#include "File_Utils.h"

void saveTableToFile(Table*table, char* filename)
{
	
	FILE* file = fopen(filename, "w");

	if (!IfFileOpen(file))	return;

    writeTableName(file, table);
    writeColName(file, table);
    writeColTypes(file, table);
    writeRows(file, table);

	fclose(file);
	printf("Table saved as text to %s\n", filename);
}

Table* loadTableFromFile(char* filename) {
    FILE* file = fopen(filename, "r");
    if (!IfFileOpen(file)) return NULL;

    Table* table = (Table*)malloc(sizeof(Table));
    if (!table) { fclose(file); return NULL; }

    table->columnCount = 0;
    table->rowCount = 0;
    table->columns = NULL;
    table->rows = NULL;



    if (!readTableName(file,table) || !readColumns(file, table) || !readColumnTypes(file, table)) {
        free(table->columns);
        free(table);
        fclose(file);
        return NULL;
    }

    while (1) {
        int res = readRow(file, table);
        if (res == 0) break;      
        if (res == -1) {            
            for (int r = 0; r < table->rowCount; r++) free(table->rows[r].fields);
            free(table->rows);
            free(table->columns);
            free(table);
            fclose(file);
            return NULL;
        }
    }

    fclose(file);
    return table;
}
