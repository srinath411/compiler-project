#include<stdio.h>
#include<stdlib.h>
#include "lexer/token_stream.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "parser/grammar.h"
#include "parser/parse_table.h"
#define INPUT_FILE "input_file.txt"
#define GRAMMAR_FILE "Grammar.txt"

int getChoice();
void exitCompiler();
void flushCompiledCode();
int compiledCode;

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
                flushCompiledCode();
                tokenizeCode(INPUT_FILE);
                parseTokens();
                compiledCode = 1;
                break;
            case 2:
                printFirstSet();
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

int getChoice() {
    int choice = 0;
    printf("0: Exit\n");
    printf("1: Compile and print errors\n");
    printf("2: Print first and follow sets\n");
    printf("3: Print token stream\n");
    printf("4: Print parse tree\n");
    printf("\nEnter your choice: ");
    scanf("%d", &choice);
    return choice;
}

void flushCompiledCode() {
    if (compiledCode) {
        freeTokenStream();
        freeParseTree();
    }
}

void exitCompiler() {
    flushCompiledCode();
    freeGrammarRules();
    freeTokenAndNonTerminalTables();
    freeFirstAndFollowSets();
    freeParseTable();
}