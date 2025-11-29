#ifndef _PARSER_H_
#define _PARSER_H_

char** tokenize(const char* input, int* count);
char* detokenize(const char** input, int count);

int isIfNotExistsUsed(char ** argv, int argSize);
char* extractName(char ** argv, int argc, int ifExists);
int isBracketsExists(const char ** argv, int argc, int ifNotExists);

char *** extractInnerArgs(const char ** argv,int argc,int *innerArgs);

char** extractSelectList(const char** argv, int argc, int *listArgs);

int extractTableName(const char** argv, int argc, char* outBuffer, size_t bufSize);

char ** extractColumnsToInsert(const char** argv, int argc, int startPos, int* columnsSize);
char *** extractedValuesToInsert(const char ** argv,int argc, int startPos, int *valuesSize, int columnCount);

#endif

