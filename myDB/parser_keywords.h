#ifndef _PARSER_KEYWORDS_H_
#define _PARSER_KEYWORDS_H_

#define BUFFER_SIZE 512


extern const char* reservedWords[];
extern const int reservedWordsCount;

int isReservedWord(const char* word);

#endif

