#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "grammar.h"
#include "../utils/hash_table.h"
#include "../utils/scanner.h"

GrammarEle* grammar[NUM_RULES];

int firstOccInRules[NUM_NON_TERMINALS];
int lastOccInRules[NUM_NON_TERMINALS];

int isInRanges(char c, char low, char high) {
    return (c >= low) && (c <= high);
}

int getFirstOccurence(NonTerminal nonTerminal) {
    return firstOccInRules[nonTerminal];
}

int getLastOccurence(NonTerminal nonTerminal) {
    return lastOccInRules[nonTerminal];
}

void setFirstOccurence(NonTerminal nonTerminal, int ruleNo) {
    if (firstOccInRules[nonTerminal] == -1) {
        firstOccInRules[nonTerminal] = ruleNo;
    }
}

GrammarEle* getGrammarRule(int ruleNo) {
    return grammar[ruleNo];
}

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
                if (isInRanges(c, 'a', 'z')) {
                    scanState = 1;
                } else if (isInRanges(c, 'A', 'Z')) {
                    scanState = 2;
                } else {
                    switch(c) {
                        case EOF:
                            break;
                        case '\n':
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
                if (!isInRanges(c, 'a', 'z') && !isInRanges(c, 'A', 'Z')) {
                    retractChar();
                    GrammarEle* ele = (GrammarEle*) malloc (sizeof(GrammarEle));
                    ele ->isTerminal = 0;
                    ele ->next = NULL;
                    char* str = getLexemeFromBuf();
                    NonTerminal nonTerminal = findNonTerminalFromString(str);
                    if (nonTerminal == -1) {
                        printf("Error %s\n", str);
                        return;
                    }
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
                if (!isInRanges(c, 'A', 'Z') && c != '_') {
                    retractChar();
                    GrammarEle* ele = (GrammarEle*) malloc (sizeof(GrammarEle));
                    ele ->isTerminal = 1;
                    ele ->next = NULL;
                    char* str = getLexemeFromBuf();
                    Token token = findTokenFromStr(str);
                    if (token == -1) {
                        printf("Error %s\n", str);
                        return;
                    }
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

void printGrammarRules() {
    for(int i = 0; i < NUM_RULES; i++) {
        GrammarEle* head = grammar[i];
        printf("Rule %d: ", i);
        while(head != NULL) {
            if (head ->isTerminal) {
                printf("%s ", getTokenStr(head ->symbol ->token));
            } else {
                printf("%s ", getNonTerminalStr(head ->symbol ->nonTerminal));
            }
            head = head ->next;
        }
        printf("\n");
    }
}