#ifndef _PARSER_H_
#define _PARSER_H_

char** split(char* input, int* argSize);
int isIfNotExistsUsed(char ** argv, int argSize);
char* extractName(char ** argv, int argc, int ifExists);
int isReservedWord(const char* word);

#endif

