#include<stdio.h>
#include<stdlib.h>
#include "lexer/token_stream.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "parser/grammar.h"
#include "parser/parse_table.h"
#define INPUT_FILE "testcases/4w.txt"
#define GRAMMAR_FILE "Grammar.txt"

int getChoice();
void exitCompiler();
void flushCompiledCode();
int compiledCode; // Indicates whether code has been compiled

int main() {
    initGrammarRules(GRAMMAR_FILE);
    computeParseTable();
    compiledCode = 0;
    while(1) {
        int choice = getChoice();
        switch (choice) {
            case 0:
                exitCompiler();
                return 0;
            case 1:
                // Flush first to save memory
                flushCompiledCode();
                tokenizeCode(INPUT_FILE);
                parseTokens();
                compiledCode = 1;
                break;
            case 2:
                printFirstAndFollowSets();
                break;
            case 3:
                printTokenStream();
                break;
            case 4:
                printParseTree();
                break;
            default:
                printf("Invalid choice\n");
        }
        printf("\n");
    }
    return 0;
}

/*
 * Get user's choice
 */
int getChoice() {
    int choice = 0;
    printf("0: Exit\n");
    printf("1: Compile and print errors\n");
    printf("2: Print first and follow sets\n");
    printf("3: Print token stream\n");
    printf("4: Print parse tree\n");
    printf("\nEnter your choice: ");
    scanf("%d", &choice);
    printf("\n");
    return choice;
}

/*
 * Flush data structures to enable recompiling
 */
void flushCompiledCode() {
    if (compiledCode) {
        freeTokenStream();
        freeParseTree();
    }
}

/*
 * Free data structures before exiting code
 */
void exitCompiler() {
    flushCompiledCode();
    freeGrammarRules();
    freeTokenAndNonTerminalTables();
    freeFirstAndFollowSets();
    freeParseTable();
}