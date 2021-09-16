#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "grammar.h"
#include "../utils/helper_functions.h"
#include "../utils/hash_table.h"
#include "../utils/scanner.h"

GrammarEle* grammar[NUM_RULES];

int firstOccInRules[NUM_NON_TERMINALS]; // Rule number having first occurence of non terminal
int lastOccInRules[NUM_NON_TERMINALS]; // Rule number having last occurence of non terminal

/*
 * Returns rule no having first occurence of non terminal
 */
int getFirstOccurence(NonTerminal nonTerminal) {
    return firstOccInRules[nonTerminal];
}

/*
 * Returns rule no having last occurence of non terminal
 */
int getLastOccurence(NonTerminal nonTerminal) {
    return lastOccInRules[nonTerminal];
}

/*
 * If nonterminal has not appeared before, sets ruleNo as its first occurence
 */
void setFirstOccurence(NonTerminal nonTerminal, int ruleNo) {
    if (firstOccInRules[nonTerminal] == -1) {
        firstOccInRules[nonTerminal] = ruleNo;
    }
}

/*
 * Returns the linked list of Grammar elements in a rule
 */
GrammarEle* getGrammarRule(int ruleNo) {
    return grammar[ruleNo];
}

/*
 * Scans grammar file and populates grammar data structure
 * Non terminals must be written in camel case
 * Tokens must begin with capital letter and contain capital letters or underscores
 */
void scanAndPopulateGrammar(char* filename) {
    int err = initScanner(filename);
    if (err) {
        printf("Error opening file: %s\n", filename);
        return;
    }
    int ruleNo = 0, scanState = 0;
    GrammarEle* last = NULL;
    char c = '\0';
    while (c != EOF) {
        switch(scanState) {
            case 0:
                c = nextChar();
                if (isInRange(c, 'a', 'z')) {
                    // Should be non terminal
                    scanState = 1;
                } else if (isInRange(c, 'A', 'Z')) {
                    // Should be token
                    scanState = 2;
                } else {
                    switch(c) {
                        case EOF:
                            break;
                        case '\n':
                            // One rule per line
                            ruleNo++;
                            skipChar();
                            break;
                        default:
                            skipChar();
                    }
                }
                break;
            case 1:
                c = nextChar();
                if (!isInRange(c, 'a', 'z') && !isInRange(c, 'A', 'Z')) {
                    // Non terminal obtained
                    retractChar();
                    GrammarEle* ele = (GrammarEle*) malloc (sizeof(GrammarEle));
                    ele ->isTerminal = 0;
                    ele ->next = NULL;
                    char* str = getLexemeFromBuf();
                    NonTerminal nonTerminal = findNonTerminalFromString(str);
                    ele ->symbol = (Symbol*) malloc (sizeof(Symbol));
                    ele ->symbol ->nonTerminal = nonTerminal;
                    if (grammar[ruleNo] == NULL) {
                        grammar[ruleNo] = ele;
                        setFirstOccurence(nonTerminal, ruleNo);
                        lastOccInRules[nonTerminal] = ruleNo;
                    } else {
                        last ->next = ele;
                    }
                    last = ele;
                    scanState = 0;
                    free(str);
                    str = NULL;
                }
                break;
            case 2:
                c = nextChar();
                if (!isInRange(c, 'A', 'Z') && c != '_') {
                    // Terminal obtained
                    retractChar();
                    GrammarEle* ele = (GrammarEle*) malloc (sizeof(GrammarEle));
                    ele ->isTerminal = 1;
                    ele ->next = NULL;
                    char* str = getLexemeFromBuf();
                    Token token = findTokenFromStr(str);
                    ele ->symbol = (Symbol*) malloc (sizeof(Symbol));
                    ele ->symbol ->token = token;
                    last ->next = ele;
                    last = ele;
                    scanState = 0;
                    free(str);
                    str = NULL;
                }
                break;
        }
    }
    closeScanner();
}
/*
 * Initializes grammar data structure using grammar file
 */
void initGrammarRules(char* filename) {
    for(int i = 0; i < NUM_NON_TERMINALS; i++) {
        firstOccInRules[i] = -1;
        lastOccInRules[i] = -1;
    }
    for(int i = 0; i < NUM_RULES; i++) {
        grammar[i] = NULL;
    }
    storeAllSymbols();
    scanAndPopulateGrammar(filename);
}

/*
 * Deallocates memory from the grammar data structure
 */
void freeGrammarRules() {
    for(int i = 0; i < NUM_RULES; i++) {
        GrammarEle* head = grammar[i];
        while(head != NULL) {
            GrammarEle* temp = head;
            head = head ->next;
            free(temp ->symbol);
            free(temp);
            temp = NULL;
        }
        grammar[i] = NULL;
    }
}