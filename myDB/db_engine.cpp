#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "db_engine.h"
#include "File_Utils.h"

void saveTableToFile(Table*table, char* filename)
{
	
	FILE* file = fopen(filename, "w");

	if (!IfFileOpen(file))	return;


	for (int i = 0; i < table->columnCount; i++) {
		fprintf(file, "%s", table->columns[i].name);
		if (i < table->columnCount - 1) fprintf(file, ";");
	}

	fprintf(file, "\n");

	for (int i = 0; i < table->rowCount; i++) {
		for (int j = 0; j < table->columnCount; j++) {
			Field f = table->rows[i].fields[j];
			
			switch (f.type)
			{
			case INT: fprintf(file, "%d\t", f.iVal); break;
			case FLOAT: printf("%.2f\t", f.fVal); break;
			case CHAR: printf("%s\t", f.sVal); break;

			default:
				break;
			}
			if (j < table->columnCount - 1) fprintf(file, ";");
		}
		fprintf(file, "\n");
	}

	fclose(file);
	printf("Table saved as text to %s\n", filename);
}

Table* loadTableFromFile(char* filename)
{
	FILE* file = fopen(filename, "r");

	if (!IfFileOpen(file))	return NULL;

	Table* table = (Table*)malloc(sizeof(Table));



	fclose(file);
}
