#include<stdio.h>
#include<stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "tokens.h"
#include "grammar.h"
#include "parse_table.h"

int main() {
    tokenizeCode("input_file.txt");
    // tokenEle* tokenStream = getTokenFromStream();
    // while(tokenStream != NULL) {
    //     if (tokenStream ->token == NUM) {
    //         printf("Line %d: %d %d\n", tokenStream ->lineNo, tokenStream ->lexeme ->num, tokenStream ->token);
    //     } else if (tokenStream ->token == R_NUM) {
    //         printf("Line %d: %f %d\n", tokenStream ->lineNo, tokenStream ->lexeme ->rnum, tokenStream ->token);
    //     } else {
    //         printf("Line %d: %s %d\n", tokenStream ->lineNo, tokenStream ->lexeme ->str, tokenStream ->token);
    //     }
    //     free(tokenStream ->lexeme);
    //     free(tokenStream);
    //     tokenStream = getTokenFromStream();
    // }
    initGrammarRules();
    // printTokenAndNonTerminalTables();
    // printGrammarRules();
    computeParseTable();
    // printFirstSet();
    parseTokens();
    freeFirstAndFollowSets();
    freeTokenAndNonTerminalTables();
    freeGrammarRules();
    return 0;
}