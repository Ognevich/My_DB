#ifndef _PARSER_CREATE_H_
#define _PARSER_CREATE_H_

int isIfNotExistsUsed(char** argv, int argSize);
char* extractName(char** argv, int argc, int ifNotExists);
int isBracketsExists(const char** argv, int argc, int ifNotExists);
char*** extractInnerArgs(const char** argv, int argc, int* innerArgs);

#endif

