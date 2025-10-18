#include "table.h"
#include "db_engine.h"
#include <stdio.h>
#include <string.h>

int main() {

	//Table* t = createTable("students");

	//addColumn(t, "ID", INT, sizeof(int));
	//addColumn(t, "Name", CHAR, 50);
	//addColumn(t, "Grade", FLOAT, sizeof(float));

 //   Field row1[3];
 //   row1[0].type = INT; row1[0].iVal = 1;
 //   row1[1].type = CHAR; strcpy(row1[1].sVal, "Alice");
 //   row1[2].type = FLOAT; row1[2].fVal = 89.5;
 //   insertRow(t, row1);

 //   Field row2[3];
 //   row2[0].type = INT; row2[0].iVal = 2;
 //   row2[1].type = CHAR; strcpy(row2[1].sVal, "Bob");
 //   row2[2].type = FLOAT; row2[2].fVal = 76.3;
 //   insertRow(t, row2);

	Table* t = loadTableFromFile("file.txt");
	printTable(t);

    freeTable(t);

	return 0;
}