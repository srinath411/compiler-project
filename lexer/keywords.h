#ifndef KEYWORDS_H
#define KEYWORDS_H

#include "../common/symbols.h"

void initKeywordTable();
void printKeywordTable();
Token getKeywordIfPresent(char* lexeme);
void freeKeywordTable();

#endif