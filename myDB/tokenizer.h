#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

char**	tokenize(const char* input, int* count);
char*	detokenize(const char** input, int count);

void	spaceTokenize(int* bi, char* buffer, char*** tokens, const char** p, int* count);
void	specialSymbolTokenize(int* bi, char* buffer, char*** tokens, const char** p, int* count);
void	symbolTokenize(int* bi, char* buffer, const char** p);
#endif
