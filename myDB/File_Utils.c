#define _CRT_SECURE_NO_WARNINGS
#include "File_Utils.h"

int IfFileOpen(FILE* file)
{
	if (!file) {
		perror("Can't open file f");
		return 0;
	}
	return 1;
}

void writeTableName(FILE* file, Table* table)
{
	fprintf(file, "%s", table->name);
	fprintf(file, "\n");
}

void writeColName(FILE* file, Table* table)
{
	for (int i = 0; i < table->columnCount; i++) {
		fprintf(file, "%s", table->columns[i].name);
		if (i < table->columnCount - 1) fprintf(file, ";");
	}
	fprintf(file, "\n");
}

void writeColTypes(FILE* file, Table* table)
{
	for (int i = 0; i < table->columnCount; i++) {
		switch (table->columns[i].type)
		{
		case INT: fprintf(file, "INT"); break;
		case FLOAT: fprintf(file, "FLOAT"); break;
		case CHAR: fprintf(file, "CHAR"); break;
		default:
			break;
		}
		if (i < table->columnCount - 1) fprintf(file, ";");
	}
	fprintf(file, "\n");
}

void writeRows(FILE* file, Table* table)
{
	for (int i = 0; i < table->rowCount; i++) {
		for (int j = 0; j < table->columnCount; j++) {
			Field f = table->rows[i].fields[j];

			switch (f.type)
			{
			case INT: fprintf(file, "%d\t", f.iVal); break;
			case FLOAT: fprintf(file, "%.2f\t", f.fVal); break;
			case CHAR: fprintf(file, "%s\t", f.sVal); break;

			default:
				break;
			}
			if (j < table->columnCount - 1) fprintf(file, ";");
		}
		fprintf(file, "\n");
	}

}

int readTableName(FILE* file, Table* table)
{
	char line[256];
	if (!fgets(line, sizeof(line), file)) return 0;
	line[strcspn(line, "\n")] = 0;

	strncpy(table->name, line, sizeof(table->name) - 1);
	table->name[sizeof(table->name) - 1] = '\0';

	return 1;
}

int readColumns(FILE* file, Table* table) {
	char line[1024];
	if (!fgets(line, sizeof(line), file)) return 0;
	line[strcspn(line, "\n")] = 0;

	char* token = strtok(line, ";");
	while (token) {
		table->columns = (Column*)realloc(table->columns, sizeof(Column) * (table->columnCount + 1));
		if (!table->columns) return 0;
		Column* col = &table->columns[table->columnCount];
		strncpy(col->name, token, sizeof(col->name) - 1);
		col->name[sizeof(col->name) - 1] = '\0';
		col->type = CHAR; 
		table->columnCount++;
		token = strtok(NULL, ";");
	}
	return 1;
}

int readColumnTypes(FILE* file, Table* table)
{
	char line[1024];
	if (!fgets(line, sizeof(line), file)) return 0;
	line[strcspn(line, "\n")] = 0;

	char* token = strtok(line, ";");
	for (int i = 0; i < table->columnCount && token; i++) {
		if (strcmp(token, "INT") == 0) table->columns[i].type = INT;
		else if (strcmp(token, "FLOAT") == 0) table->columns[i].type = FLOAT;
		else table->columns[i].type = CHAR;
		token = strtok(NULL, ";");
	}
	return 1;
}

int readRow(FILE* file, Table* table)
{
	char line[1024];
	if (!fgets(line, sizeof(line), file)) return 0;
	line[strcspn(line, "\n")] = 0;

	Row row;
	row.fields = (Field*)malloc(sizeof(Field) * table->columnCount);
	if (!row.fields) return -1;

	char* token = strtok(line, ";");
	for (int j = 0; j < table->columnCount && token; j++) {
		switch (table->columns[j].type) {
		case INT: row.fields[j].iVal = atoi(token); break;
		case FLOAT: row.fields[j].fVal = atof(token); break;
		case CHAR:
			strncpy(row.fields[j].sVal, token, sizeof(row.fields[j].sVal) - 1);
			row.fields[j].sVal[sizeof(row.fields[j].sVal) - 1] = '\0';
			break;
		}
		row.fields[j].type = table->columns[j].type;
		token = strtok(NULL, ";");
	}

	table->rows = (Row*)realloc(table->rows, sizeof(Row) * (table->rowCount + 1));
	if (!table->rows) {
		free(row.fields);
		return -1;
	}

	table->rows[table->rowCount] = row;
	table->rowCount++;
	return 1;
}
