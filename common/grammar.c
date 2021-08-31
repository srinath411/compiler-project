#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "grammar.h"
#include "../common/tokens.h"
#include "../utils/hash_table.h"
#include "../utils/scanner.h"
#define TOKEN_BUCKETS 8
#define NON_TERMINAL_BUCKETS 8

GrammarEle* grammar[NUM_RULES];

char* tokenStrs[] = {"ASSIGN_OP", "COMMENT", "FIELD_ID", "ID", "NUM", "R_NUM", "FUN_ID", "RECORD_ID", "WITH", "PARAMETERS", "END",
"WHILE", "INT", "REAL", "TYPE", "MAIN", "GLOBAL", "PARAMETER", "LIST", "SQL", "SQR", "INPUT", "OUTPUT", "COMMA", "SEM", "COLON",
"DOT", "END_WHILE", "OP", "CL", "IF", "THEN", "END_IF", "READ", "WRITE", "RETURN", "PLUS", "MINUS", "MUL", "DIV", "CALL", "RECORD",
"END_RECORD", "ELSE", "AND", "OR", "NOT", "LT", "GT", "LE", "GE", "EQ", "NE", "DOLLAR", "EPS"};

char* nonTerminalStrs[] = {"program", "otherFunctions", "function", "inputParams", "outputParams", "paramList", "params", "param",
"type", "primitiveType", "userDefType", "otherParams", "stmts", "recordDefinitions", "recordDefinition", "fieldStmts", "fieldStmt",
"otherFieldStmts", "declStmts", "declStmt", "global", "otherStmts", "stmt", "assignStmt", "generalId", "fieldRef", "exp", 
"restExp", "mul", "restMul", "var", "allId", "iterStmt", "boolExp", "restBool", "andExp", "restAnd", "boolVar", "boolComp", "boolOp",
"condStmt", "optionalElse", "callStmt", "optionalReturn", "ids", "otherIds", "allIdList", "allIds", "otherAlls", "ioStmt", "returnStmt",
"mainFunction"};

hashEle** tokenTable = NULL;
hashEle** nonTerminalTable = NULL;

int firstOccInRules[NUM_NON_TERMINALS];
int lastOccInRules[NUM_NON_TERMINALS];

void storeTokenAndNonTerminalStrs() {
    tokenTable = initHashTable(tokenStrs, NUM_TOKENS, TOKEN_BUCKETS);
    nonTerminalTable = initHashTable(nonTerminalStrs, NUM_NON_TERMINALS, NON_TERMINAL_BUCKETS);
}

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
                    ele ->symbol = findEleInTable(nonTerminalTable, NON_TERMINAL_BUCKETS, str);
                    if (ele ->symbol == -1) {
                        printf("Error %s\n", str);
                        return;
                    }
                    if (grammar[ruleNo] == NULL) {
                        grammar[ruleNo] = ele;
                        setFirstOccurence(ele ->symbol, ruleNo);
                        lastOccInRules[ele ->symbol] = ruleNo;
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
                    ele ->symbol = findEleInTable(tokenTable, TOKEN_BUCKETS, str);
                    if (ele ->symbol == -1) {
                        printf("Error %s\n", str);
                        return;
                    }
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
    storeTokenAndNonTerminalStrs();
    scanAndPopulateGrammar(filename);
}

void freeTokenAndNonTerminalTables() {
    freeHashTable(tokenTable, TOKEN_BUCKETS);
    freeHashTable(nonTerminalTable, NON_TERMINAL_BUCKETS);
    tokenTable = NULL;
    nonTerminalTable = NULL;
}

void freeGrammarRules() {
    for(int i = 0; i < NUM_RULES; i++) {
        GrammarEle* head = grammar[i];
        while(head != NULL) {
            GrammarEle* temp = head;
            head = head ->next;
            free(temp);
            temp = NULL;
        }
        grammar[i] = NULL;
    }
}

//debug
void printTokenAndNonTerminalTables() {
    printHashTable(tokenTable, TOKEN_BUCKETS);
    printf("\n");
    printHashTable(nonTerminalTable, NON_TERMINAL_BUCKETS);
}

void printGrammarRules() {
    for(int i = 0; i < NUM_RULES; i++) {
        GrammarEle* head = grammar[i];
        printf("Rule %d: ", i);
        while(head != NULL) {
            if (head ->isTerminal) {
                printf("%s ", tokenStrs[head ->symbol]);
            } else {
                printf("%s ", nonTerminalStrs[head ->symbol]);
            }
            head = head ->next;
        }
        printf("\n");
    }
}

char* getNonTerminalStr(int i) {
    return nonTerminalStrs[i];
}

char* getTokenStr(int i) {
    return tokenStrs[i];
}