#ifndef GRAMMAR_H
#define GRAMMAR_H
#include "../common/symbols.h"
#define NUM_RULES 89

typedef struct GrammarEle {
    Symbol* symbol;
    int isTerminal;
    struct GrammarEle* next;
} GrammarEle;

void initGrammarRules(char* filename);
int getFirstOccurence(NonTerminal nonTerminal);
int getLastOccurence(NonTerminal nonTerminal);
GrammarEle* getGrammarRule(int ruleNo);
void freeTokenAndNonTerminalTables();
void freeGrammarRules();
void printTokenAndNonTerminalTables();

#endif