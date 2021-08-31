#ifndef PARSE_TABLE_H
#define PARSE_TABLE_H

void computeParseTable();
int getRuleNoFromTable(NonTerminal nonTerminal, Token token);
void printFirstSet();
void freeFirstAndFollowSets();

#endif