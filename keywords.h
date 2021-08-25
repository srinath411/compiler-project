#ifndef KEYWORDS_H
#define KEYWORDS_H
#include "tokens.h"

void initKeywordTable();
void printKeywordTable();
Token getKeywordIfPresent(char* lexeme);
void freeKeywordTable();

#endif