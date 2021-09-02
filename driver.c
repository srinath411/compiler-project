#include<stdio.h>
#include<stdlib.h>
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "parser/grammar.h"
#include "parser/parse_table.h"
#define INPUT_FILE "input_file.txt"
#define GRAMMAR_FILE "Grammar.txt"

int main() {
    tokenizeCode(INPUT_FILE);
    initGrammarRules(GRAMMAR_FILE);
    // printTokenAndNonTerminalTables();
    // printGrammarRules();
    computeParseTable();
    // printFirstSet();
    parseTokens();
    freeFirstAndFollowSets();
    freeParseTable();
    freeTokenAndNonTerminalTables();
    freeGrammarRules();
    return 0;
}