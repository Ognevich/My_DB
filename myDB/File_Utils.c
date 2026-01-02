#define _CRT_SECURE_NO_WARNINGS
#include "File_Utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifdef _WIN32
#include <direct.h>
#include <sys/stat.h>
#include <windows.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

int directoryExists(const char* path)
{
	struct stat st;
	if (stat(path, &st) != 0)
		return 0;

#ifdef _WIN32
	return (st.st_mode & _S_IFDIR) != 0;
#else
	return (st.st_mode & S_IFDIR) != 0;
#endif
}

int fileExists(const char* path) {
	struct stat st;
	return stat(path, &st) == 0 && !(st.st_mode & S_IFDIR);
}

int IfFileOpen(FILE* file)
{
	if (!file) {
		perror("Can't open file f");
		return 0;
	}
	return 1;
}


int createDbDirectory(const char* name)
{
	char dbPath[256];
	char metaPath[256];

	snprintf(dbPath, sizeof(dbPath), "%s/%s", DB_ROOT, name);

#ifdef _WIN32
	if (_mkdir(dbPath) != 0) {
#else
	if (mkdir(dbPath, 0755) != 0) {
#endif
		perror("Failed to create database directory");
		return 0;
	}

	snprintf(metaPath, sizeof(metaPath), "%s/%s/%s.meta", DB_ROOT, name, name);

	FILE* file = fopen(metaPath, "w");
	if (!file) {
		perror("Failed to create meta file");

#ifdef _WIN32
		_rmdir(dbPath);
#else
		rmdir(dbPath);
#endif
		return 0;
	}

	fprintf(file, "%s\n0\n", name);
	fclose(file);

	return 1;
}

void writeDatabase(FILE* file, Database* database)
{
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
		case FIELD_INT: fprintf(file, "INT"); break;
		case FIELD_FLOAT: fprintf(file, "FLOAT"); break;
		case FIELD_CHAR: fprintf(file, "CHAR"); break;
		default:
			break;
		}
		if (i < table->columnCount - 1) fprintf(file, ";");
	}
	fprintf(file, "\n");
}

void writeRow(FILE* file, Field* fields, int size)
{
	for (int i = 0; i < size; i++)
	{
		Field f = fields[i];

		switch (f.type)
		{
		case FIELD_INT: fprintf(file, "%d", f.iVal); break;
		case FIELD_FLOAT: fprintf(file, "%.2f", f.fVal); break;
		case FIELD_CHAR: fprintf(file, "%s", f.sVal); break;
		default:
			break;
		}
		if (i < size - 1) fprintf(file, ";");
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
			case FIELD_INT: fprintf(file, "%d\t", f.iVal); break;
			case FIELD_FLOAT: fprintf(file, "%.2f\t", f.fVal); break;
			case FIELD_CHAR: fprintf(file, "%s\t", f.sVal); break;

			default:
				break;
			}
			if (j < table->columnCount - 1) fprintf(file, ";");
		}
		fprintf(file, "\n");
	}

}

int readDataFromFile(AppContext* app)
{

	FILE* file = fopen(DB_ROOT, 'r');
	
	fclose(file);
}

void readDatabase(AppContext* app, const char* dbPath);

int scanDatabase(AppContext* app)
{
#ifdef _WIN32
	WIN32_FIND_DATAA findData;
	HANDLE hFind;

	char searchPath[512];
	snprintf(searchPath, sizeof(searchPath), "%s\\*", DB_ROOT);

	hFind = FindFirstFileA(searchPath, &findData);
	if (hFind == INVALID_HANDLE_VALUE) {
		printf("Cannot open DB_ROOT\n");
		return;
	}

	do {
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {

			if (strcmp(findData.cFileName, ".") == 0 ||
				strcmp(findData.cFileName, "..") == 0)
				continue;

			char dbPath[512];
			snprintf(dbPath, sizeof(dbPath), "%s\\%s", DB_ROOT, findData.cFileName);

			readDatabase(app, dbPath);
		}
	} while (FindNextFileA(hFind, &findData));

	FindClose(hFind);

#else
	DIR* dir = opendir(DB_ROOT);
	if (!dir) {
		perror("opendir DB_ROOT");
		return;
	}

	struct dirent* entry;
	while ((entry = readdir(dir)) != NULL) {

		if (strcmp(entry->d_name, ".") == 0 ||
			strcmp(entry->d_name, "..") == 0)
			continue;

		if (entry->d_type == DT_DIR) {
			char dbPath[512];
			snprintf(dbPath, sizeof(dbPath), "%s/%s", DB_ROOT, entry->d_name);

			printf("\nDB directory: %s\n", dbPath);
			readDatabase(dbPath);
		}
	}

	closedir(dir);
#endif
}

void readDatabase(AppContext* app, const char* dbpath)
{
#ifdef _WIN32
	WIN32_FIND_DATAA findData;
	HANDLE hFind;

	char searchPath[512];
	snprintf(searchPath, sizeof(searchPath), "%s\\*", dbpath);

	hFind = FindFirstFileA(searchPath, &findData);
	if (hFind == INVALID_HANDLE_VALUE)
		return;

	do {
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			continue;

		char path[512];
		snprintf(path, sizeof(path), "%s\\%s", dbpath, findData.cFileName);

		if (strstr(findData.cFileName, ".meta")) {
			readMeta(path);
		}


		if (strstr(findData.cFileName, ".tbl")) {
			readTable(path);
		}

	} while (FindNextFileA(hFind, &findData));

	FindClose(hFind);

#else
	DIR* dir = opendir(dbPath);
	if (!dir) return;

	struct dirent* entry;
	char path[512];

	while ((entry = readdir(dir)) != NULL) {

		if (strcmp(entry->d_name, ".") == 0 ||
			strcmp(entry->d_name, "..") == 0)
			continue;

		snprintf(path, sizeof(path), "%s/%s", dbPath, entry->d_name);

		if (strstr(entry->d_name, ".meta")) {
			printf("\nMETA FILE\n");
			readMeta(path);
		}

		if (strstr(entry->d_name, ".tbl")) {
			readTable(path);
		}
	}

	closedir(dir);
#endif
}

void readMeta(AppContext* app, const char* metapath)
{

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
		col->type = FIELD_CHAR;
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
		if (strcmp(token, "INT") == 0) table->columns[i].type = FIELD_INT;
		else if (strcmp(token, "FLOAT") == 0) table->columns[i].type = FIELD_FLOAT;
		else table->columns[i].type = FIELD_CHAR;
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
		case FIELD_INT: row.fields[j].iVal = atoi(token); break;
		case FIELD_FLOAT: row.fields[j].fVal = atof(token); break;
		case FIELD_CHAR:
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

int saveTableToFile(Table* table, AppContext* app, const char* name, const char*** args, int size)
{
	char tablePath[256];
	snprintf(tablePath, sizeof(tablePath), "%s/%s/%s.tbl", DB_ROOT, app->currentDatabase->name, name);

	if (fileExists(tablePath))
	{
		printf("Error: file %s already exists\n", tablePath);
		return 0;
	}

	FILE* file = fopen(tablePath, "w");
	if (!file)
		return;

	writeTableName(file, table);
	writeColName(file, table);
	writeColTypes(file, table);

	fclose(file);

}

int appendTableRowsToFile(Field* fields, int size, const char* dbName, const char* tableName)
{
	char filePath[256];
	snprintf(filePath, sizeof(filePath), "%s/%s/%s.tbl", DB_ROOT, dbName, tableName);

	FILE* file = fopen(filePath, "a");

	if (!file)
		return 0;

	writeRow(file, fields, size);

	fclose(file);

	return 1;
}


int removeDirRecursive(const char* path)
{
#ifdef _WIN32
	WIN32_FIND_DATAA ffd;
	char searchPath[MAX_PATH];
	char fullPath[MAX_PATH];

	snprintf(searchPath, MAX_PATH, "%s\\*", path);

	HANDLE hFind = FindFirstFileA(searchPath, &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
		return -1;

	do {
		if (!strcmp(ffd.cFileName, ".") || !strcmp(ffd.cFileName, ".."))
			continue;

		snprintf(fullPath, MAX_PATH, "%s\\%s", path, ffd.cFileName);

		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			removeDirRecursive(fullPath);
		}
		else
		{
			DeleteFileA(fullPath);
		}

	} while (FindNextFileA(hFind, &ffd));

	FindClose(hFind);
	return RemoveDirectoryA(path);

#else
	DIR* dir = opendir(path);
	if (!dir) return -1;

	struct dirent* entry;
	char fullPath[512];

	while ((entry = readdir(dir)) != NULL)
	{
		if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
			continue;

		snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);

		struct stat st;
		if (stat(fullPath, &st) == 0)
		{
			if (S_ISDIR(st.st_mode))
				removeDirRecursive(fullPath);
			else
				remove(fullPath);
		}
	}

	closedir(dir);
	return rmdir(path);
#endif
}

void increaseMeta(const char* dbname)
{
	char filePath[256];
	char dbName[256];
	char intBuff[256];
	int value = 0;

	snprintf(filePath, sizeof(filePath), "%s/%s/%s.meta", DB_ROOT, dbname, dbname);

	FILE* file = fopen(filePath, "r");
	if (!file) {
		perror("fopen read");
		return;
	}

	if (!fgets(dbName, sizeof(dbName), file)) {
		fclose(file);
		return;
	}

	dbName[strcspn(dbName, "\n")] = '\0';

	if (fgets(intBuff, sizeof(intBuff), file)) {
		value = (int)strtol(intBuff, NULL, 10);
	}

	fclose(file);

	FILE* wfile = fopen(filePath, "w");
	if (!wfile) {
		perror("fopen write");
		return;
	}

	fprintf(wfile, "%s\n%d\n", dbName, value + 1);
	fclose(wfile);
}
