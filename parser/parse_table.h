#ifndef PARSE_TABLE_H
#define PARSE_TABLE_H

#include "../common/symbols.h"

void computeParseTable();
int getRuleNoFromTable(NonTerminal nonTerminal, Token token);
void printFirstSet();
void freeFirstAndFollowSets();
void freeParseTable();

#endif