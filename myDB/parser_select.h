#ifndef _PARSER_SELECT_H
#define _PARSER_SELECT_H

char** extractSelectList(const char** argv, int argc, int* listArgs);
int extractTableName(const char** argv, int argc, char* outBuffer, size_t bufSize);

#endif