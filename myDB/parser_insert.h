#ifndef _PARSER_INSERT_H_
#define _PARSER_INSERT_H_

char*** extractedValuesToInsert(const char** argv, int argc, int startPos, int* valuesSize, int columnCount);
char** parseRow(const char** argv, int argc, int* index, int columnCount);
char** parseValues(const char** argv, int argc, int* index, int columnCount);
char** extractColumnsToInsert(const char** argv, int argc, int startPos, int* columnsSize);

#endif
