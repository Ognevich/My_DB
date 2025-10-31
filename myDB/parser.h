#ifndef _PARSER_H_
#define _PARSER_H_

char** tokenize(const char* input, int* count);
int isIfNotExistsUsed(char ** argv, int argSize);
char* extractName(char ** argv, int argc, int ifExists);
int isReservedWord(const char* word);

#endif

