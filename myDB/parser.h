#ifndef _PARSER_H_
#define _PARSER_H_

char** tokenize(const char* input, int* count);
void spaceTokenize(int *bi, char *buffer, char ***tokens, const char ** p,int* count);
void specialSymbolTokenize(int* bi, char* buffer, char*** tokens, const char** p, int* count);
void symbolTokenize(int * bi, char * buffer, const char **p);

int isIfNotExistsUsed(char ** argv, int argSize);
char* extractName(char ** argv, int argc, int ifExists);
int isReservedWord(const char* word);
int isValidName(const char* word);
int isBracketsExists(const char ** argv, int argc, int ifNotExists);

char *** extractInnerArgs(const char ** argv,int argc,int *innerArgs);

#endif

