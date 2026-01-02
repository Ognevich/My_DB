#define _CRT_SECURE_NO_WARNINGS
#include "database.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "File_Utils.h"

Database* createDatabase(const char* name, int createFolder)
{
    if (!name) return NULL;

    if (createFolder) {
        if (!createDbDirectory(name))
            return NULL;
    }

    Database* db = malloc(sizeof(Database));
    if (!db) {
        perror("malloc failed");
        return NULL;
    }

    strncpy(db->name, name, sizeof(db->name) - 1);
    db->name[sizeof(db->name) - 1] = '\0'; 

    db->tables = NULL;
    db->tableCount = 0;

    return db;
}

void addTable(Database* db, Table* table) {
	db->tables = realloc(db->tables, sizeof(Table*) * (db->tableCount+1));
	db->tables[db->tableCount++] = table;
}

Table* findTable(Database* db, const char* tableName)
{
    if (!db || !tableName) {
        fprintf(stderr, "findTable: invalid arguments\n");
        return NULL;
    }

    if (db->tableCount == 0) {
        fprintf(stderr, "No tables exist in the database\n");
        return NULL;
    }

    for (int i = 0; i < db->tableCount; i++) {
        if (strcmp(db->tables[i]->name, tableName) == 0) {
            return db->tables[i];
        }
    }

    fprintf(stderr, "Table '%s' not found\n", tableName);
    return NULL;
}

void deleteTable(Database* db, const char* tableName) {
    if (!db || !tableName) {
        fprintf(stderr, "deleteTable: invalid arguments\n");
        return;
    }

    int index = -1;

    for (int i = 0; i < db->tableCount; i++) {
        if (strcmp(db->tables[i]->name, tableName) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        fprintf(stderr, "deleteTable: table '%s' not found\n", tableName);
        return;
    }

    freeTable(db->tables[index]);

    for (int i = index; i < db->tableCount - 1; i++) {
        db->tables[i] = db->tables[i + 1];
    }

    db->tableCount--;

    if (db->tableCount > 0) {
        Table** tmp = realloc(db->tables, sizeof(Table*) * db->tableCount);
        if (tmp) {
            db->tables = tmp;
        }
    }
    else {
        free(db->tables);
        db->tables = NULL;
    }

    printf("Table '%s' deleted successfully\n", tableName);

}

void printDatabase(Database* db)
{
    if (!db) {
        fprintf(stderr, "printDatabase: invalid arguments\n");
        return;
    }

    printf("======%s======\n", db->name);
    printf("Total Tables: %d\n", db->tableCount);

    for (int i = 0; i < db->tableCount; i++) {
        printf("Table %d\n", i+1);
        printTable(db->tables[i]);
        printf("\n");
    }

    printf("=======================\n");
}

int isTableExists(Database* db, const char* name)
{
    for (int i = 0; i < db->tableCount; i++)
        if (strcmp(db->tables[i]->name, name) == 0)     return 1;

    return 0;
}

void freeDatabase(Database* db)
{
    if (!db)
        return;
    for (int i = 0; i < db->tableCount; i++) {
        if (db->tables[i]) {
            freeTable(db->tables[i]);
        }
    }
    free(db->tables);
    db->tables = NULL;

    free(db);
}


