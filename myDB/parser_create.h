#ifndef _PARSER_CREATE_H_
#define _PARSER_CREATE_H_
#include "parser_keywords.h"

int isIfNotExistsUsed(char** argv, int argSize);
void extractName(char** argv, int argc,char ** name, int ifNotExists);
int isBracketsExists(const char** argv, int argc, int ifNotExists);
SqlError extractInnerArgs(const char** argv, int argc, char**** outResult, int* innerArgs);

#endif

