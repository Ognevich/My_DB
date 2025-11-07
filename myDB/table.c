#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"

Table* createTable(const char* name) {
    Table* t = malloc(sizeof(*t));
    if (!t) {
        fprintf(stderr, "Memory allocation failed for Table\n");
        return NULL;
    }

    strncpy(t->name, name, sizeof(t->name) - 1);
    t->name[sizeof(t->name) - 1] = '\0';

    t->columns = NULL;
    t->columnCount = 0;
    t->rows = NULL;
    t->rowCount = 0;

    return t;
}

Table* initNewTable(const char* name) {
    Table* table = createTable(name);
    if (!table) {
        logMessage(LOG_ERROR, "Failed to create table structure");
    }
    return table;
}

int fillTableColumns(Table* table, char*** innerArgs, int innerSize) {
    for (int i = 0; i < innerSize; i++) {
        char* columnName = innerArgs[i][0];
        FieldType type = StrToField(innerArgs[i][1]);

        if (type == NONE) {
            printf("Error: Incorrect data type '%s'\n", innerArgs[i][1]);
            return 0;
        }

        int columnSize = defineColumnSize(type);
        addColumn(table, columnName, type, columnSize);
    }
    return 1;

}

void addColumn(Table* table, const char* name, FieldType type, int size) {
    if (!table) {
        fprintf(stderr, "Table pointer is NULL\n");
        return;
    }

    Column* newColumns = realloc(table->columns, sizeof(Column) * (table->columnCount + 1));
    if (!newColumns) {
        fprintf(stderr, "Memory allocation failed while adding column\n");
        return;
    }

    table->columns = newColumns;
    Column* col = &table->columns[table->columnCount];

    strncpy(col->name, name, sizeof(col->name) - 1);
    col->name[sizeof(col->name) - 1] = '\0';

    col->type = type;
    col->size = size;

    table->columnCount++;
}

void insertRow(Table* table, Field* fields) {
    if (!table) {
        fprintf(stderr, "Table pointer is NULL\n");
        return;
    }

    Row* newRows = realloc(table->rows, sizeof(Row) * (table->rowCount + 1));

    if (!newRows) {
        fprintf(stderr, "Memory allocation failed while adding row\n");
        return;
    }

    table->rows = newRows;
    Row* row = &table->rows[table->rowCount];

    row->fields = malloc(sizeof(Field) * table->columnCount);

    for (int i = 0; i < table->columnCount; i++) {
        row->fields[i] = fields[i];
    }
    table->rowCount++;
}

void deleteRow(Table* table, int rowIndex)
{
    if (rowIndex < 0 || rowIndex >= table->rowCount) {
        perror("Index out of range");
        return;
    }

    free(table->rows[rowIndex].fields);

    for (int i = rowIndex; i < table->rowCount - 1; i++) {
        table->rows[i] = table->rows[i + 1];
    }

    table->rowCount--;

    if (table->rowCount > 0)
        table->rows = (Row*)realloc(table->rows, sizeof(Row) * table->rowCount);
    else {
        free(table->rows);
        table->rows = NULL;
    }
}

void updateRow(Table* table, int rowIndex, Field* newValues)
{
    if (rowIndex < 0 || rowIndex >= table->rowCount) {
        perror("Index out of range");
        return;
    }

    for (int i = 0; i < table->columnCount; i++) {
        switch (table->columns[i].type)
        {
        case INT:
            table->rows[rowIndex].fields[i].iVal = newValues[i].iVal;
            break;
        case FLOAT:
            table->rows[rowIndex].fields[i].fVal = newValues[i].fVal;
            break;
        case CHAR:
            strncpy(table->rows[rowIndex].fields[i].sVal, newValues[i].sVal, sizeof(table->rows[rowIndex].fields[i].sVal) - 1);
            table->rows[rowIndex].fields[i].sVal[sizeof(table->rows[rowIndex].fields[i].sVal) - 1] = '\0';
            break;
        default:
            break;
        }
        table->rows[rowIndex].fields[i].type = table->columns[i].type; 
    }
}

void printTable(Table* table)
{
    printf("Table: %s\n", table->name);

    for (int i = 0; i < table->columnCount; i++) {
        printf("%s\t", table->columns[i].name);
    }

    printf("\n");

    for (int i = 0; i < table->rowCount; i++) {
        for (int j = 0; j < table->columnCount; j++) {

            Field f = table->rows[i].fields[j];
               
            switch (f.type)
            {
            case INT: printf("%d\t", f.iVal); break;
            case FLOAT: printf("%.2f\t", f.fVal); break;
            case CHAR: printf("%s\t", f.sVal); break;
            default:
                break;
            }
           }
        printf("\n");
    }
   

}

void printSelectedColumns(Table* table, const char** columns, int columnsCount)
{
    if (!table || !columns || columnsCount <= 0) {
        fprintf(stderr, "Error: invalid arguments to printSelectedColumns.\n");
        return;
    }

    int* selectedIndices = malloc(columnsCount * sizeof(int));
    if (!selectedIndices) {
        fprintf(stderr, "Memory allocation failed for selectedIndices.\n");
        return;
    }

    printf("Table: %s\n", table->name);

    for (int i = 0; i < columnsCount; i++) {
        int found = 0;
        for (int j = 0; j < table->columnCount; j++) {
            if (strcmp(columns[i], table->columns[j].name) == 0) {
                selectedIndices[i] = j;
                found = 1;
                break;
            }
        }
        if (!found) {
            fprintf(stderr, "Warning: column '%s' not found in table '%s'.\n",
                columns[i], table->name);
            selectedIndices[i] = -1;
        }
    }

    for (int i = 0; i < columnsCount; i++) {
        if (selectedIndices[i] != -1)
            printf("%s\t", columns[i]);
    }
    printf("\n");

    for (int i = 0; i < table->rowCount; i++) {
        for (int j = 0; j < columnsCount; j++) {
            int colIdx = selectedIndices[j];
            if (colIdx == -1) continue;

            Field f = table->rows[i].fields[colIdx];
            switch (f.type) {
            case INT:   printf("%d\t", f.iVal); break;
            case FLOAT: printf("%.2f\t", f.fVal); break;
            case CHAR:  printf("%s\t", f.sVal); break;
            default:    printf("?\t"); break;
            }
        }
        printf("\n");
    }

    free(selectedIndices);
}

void freeTable(Table* table)
{
    if (!table) {
        return;
    }

    for (int i = 0; i < table->rowCount; i++) {

        free(table->rows[i].fields);

    }

    free(table->rows);
    free(table->columns);
    free(table);

}

int isColumnsExists(const char** selectArray, int selectArraySize, Table* table, int* isAsterisk)
{
    if (strcmp(selectArray[0], "*") == 0)
    {
        *isAsterisk = 1;
        return 1;
    }

    for (int i = 0; i < selectArraySize; i++) {
        int isColumn = 0;  
        for (int j = 0; j < table->columnCount; j++) {
            if (strcmp(selectArray[i], table->columns[j].name) == 0) {
                isColumn = 1;
                break;
            }
        }
        if (!isColumn) {
            return 0; 
        }
    }
    return 1; 
}

