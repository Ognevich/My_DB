#ifndef _PARSER_INSERT_H_
#define _PARSER_INSERT_H_

int extractedValuesToInsert(const char** argv, int argc, int startPos, char**** outValues, int* valuesSize, int columnCount);
int parseRow(const char** argv, int argc, int* index, char*** outRow, int columnCount);
int parseValues(const char** argv, int argc, int* index, char*** outValues, int columnCount);
int extractColumnsToInsert(const char** argv, int argc, int startPos, char*** outColumn, int* columnsSize);

#endif
