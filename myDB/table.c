#include "table.h"

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


