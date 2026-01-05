#ifndef _PARSER_DROP_H_
#define _PARSER_DROP_H_

int ifExistsUsed(const char  ** argv, int argc);
void extractObjName(const char ** argv, int argc, const char ** name, int isExists);

#endif
