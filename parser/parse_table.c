#include<stdio.h>
#include<stdlib.h>
#include "../common/symbols.h"
#include "grammar.h"
#include "../utils/bit_array.h"

int** firstSet = NULL;
int** followSet = NULL;
int** parseTable = NULL;

int* tempFollow;
int* doneFirstCalc;
int* before;

int** initSet() {
    int** set = (int**) malloc (NUM_NON_TERMINALS * sizeof(int*));
    for(int i = 0; i < NUM_NON_TERMINALS; i++) {
        set[i] = initBitArr(NUM_TOKENS);
    }
    return set;
}

void freeSet(int** set) {
    for(int i = 0; i < NUM_NON_TERMINALS; i++) {
        free(set[i]);
        set[i] = NULL;
    }
    free(set);
    set = NULL;
}

void initFirstAndFollowSets() {
    firstSet = initSet();
    followSet = initSet();
    tempFollow = initBitArr(NUM_TOKENS);
    before = initBitArr(NUM_TOKENS);
    doneFirstCalc = initBitArr(NUM_NON_TERMINALS);
}

void initParseTable() {
    parseTable = (int**) malloc(NUM_NON_TERMINALS * sizeof(int*));
    for(int i = 0; i < NUM_NON_TERMINALS; i++) {
        parseTable[i] = (int*) malloc(NUM_TOKENS * sizeof(int));
        for(int j = 0; j < NUM_TOKENS; j++) {
            parseTable[i][j] = -1;
        }
    }
}

void calcFirstSet(NonTerminal nonTerminal) {
    if (isBitSet(doneFirstCalc, nonTerminal)) {
        return;
    }
    int rulePos = getFirstOccurence(nonTerminal);
    int lastOccurence = getLastOccurence(nonTerminal);
    int isEpsilonPresent = 0;
    if (rulePos == -1 || lastOccurence < rulePos) {
        printf("Error\n");
        return;
    }
    for(int i = rulePos; i <= lastOccurence; i++) {
        GrammarEle* ele = getGrammarRule(i);
        if (ele == NULL || ele ->next == NULL) {
            printf("Error\n");
            return;
        }
        ele = ele ->next;
        while(ele != NULL) {
            if (ele ->isTerminal) {
                setBit(firstSet[nonTerminal], ele ->symbol ->token);
                if (ele ->symbol ->token == EPS) {
                    isEpsilonPresent = 1;
                }
                break;
            } else {
                calcFirstSet(ele ->symbol ->nonTerminal);
                computeBitArrUnion(firstSet[nonTerminal], firstSet[ele ->symbol ->nonTerminal], NUM_TOKENS);
                if (!isBitSet(firstSet[ele ->symbol ->nonTerminal], EPS)) {
                    break;
                } else if (ele ->next == NULL) {
                    isEpsilonPresent = 1;
                }
            }
            clearBit(firstSet[nonTerminal], EPS);
            ele = ele ->next;
        }
    }
    if (isEpsilonPresent) {
        setBit(firstSet[nonTerminal], EPS);
    }
    setBit(doneFirstCalc, nonTerminal);
}

void computeFirstSet() {
    for(int i = 0; i < NUM_NON_TERMINALS; i++) {
        calcFirstSet(i);
    }
}

int computeFollowInRule(NonTerminal head, GrammarEle* ele) {
    int setChanged = 0;
    if (ele == NULL) {
        return 0;
    }
    setChanged = computeFollowInRule(head, ele ->next);
    if (ele ->isTerminal) {
        clearAllBits(tempFollow, NUM_TOKENS);
        setBit(tempFollow, ele ->symbol ->token);
        return setChanged;
    } else {
        copyBitArr(before, followSet[ele ->symbol ->nonTerminal], NUM_TOKENS);
        if (ele ->next == NULL) {
            computeBitArrUnion(followSet[ele ->symbol ->nonTerminal], followSet[head], NUM_TOKENS);
            if (isBitSet(firstSet[ele ->symbol ->nonTerminal], EPS)) {
                copyBitArr(tempFollow, followSet[head], NUM_TOKENS);
            }
        } else if (ele ->next ->isTerminal) {
            setBit(followSet[ele ->symbol ->nonTerminal], ele ->next ->symbol ->token);
        } else {
            computeBitArrUnion(followSet[ele ->symbol ->nonTerminal], firstSet[ele ->next ->symbol ->nonTerminal], NUM_TOKENS);
            if (isBitSet(followSet[ele ->symbol ->nonTerminal], EPS)) {
                computeBitArrUnion(followSet[ele ->symbol ->nonTerminal], tempFollow, NUM_TOKENS);
                clearBit(followSet[ele ->symbol ->nonTerminal], EPS);
            }
        }
        if (!bitArrsEqual(before, followSet[ele ->symbol ->nonTerminal], NUM_TOKENS)) {
            setChanged = 1;
        }
        if (!isBitSet(firstSet[ele ->symbol ->nonTerminal], EPS)) {
            copyBitArr(tempFollow, firstSet[ele ->symbol ->nonTerminal], NUM_TOKENS);
            clearBit(tempFollow, EPS);
        } else {
            computeBitArrUnion(tempFollow, firstSet[ele ->symbol ->nonTerminal], NUM_TOKENS);
        }
        return setChanged;
    }
}

void computeFollowSet() {
    setBit(followSet[program], DOLLAR);
    int setChanged = 1;
    while(setChanged) {
        setChanged = 0;
        for(int i = 0; i < NUM_RULES; i++) {
            GrammarEle* ele = getGrammarRule(i);
            if (ele == NULL || ele ->next == NULL) {
                printf("Error\n");
                return;
            }
            NonTerminal head = ele ->symbol ->nonTerminal;
            ele = ele ->next;
            clearAllBits(tempFollow, NUM_TOKENS);
            setChanged = setChanged || computeFollowInRule(head, ele);
        }
    }
}

void addSetToTable(NonTerminal nonTerminal, int* set, int ruleNo) {
    for(int i = 0; i < NUM_TOKENS; i++) {
        if (isBitSet(set, i)) {
            if (parseTable[nonTerminal][i] != -1) {
                printf("ERROR!!!\n");
                return;
            } else {
                parseTable[nonTerminal][i] = ruleNo;
            }
        }
    }
}

void addFirstSetToTable(NonTerminal nonTerminal, GrammarEle* ele, int ruleNo) {
    clearAllBits(tempFollow, NUM_TOKENS);
    while(ele != NULL) {
        if (ele ->isTerminal) {
            setBit(tempFollow, ele ->symbol ->token);
            break;
        } else {
            computeBitArrUnion(tempFollow, firstSet[ele ->symbol ->nonTerminal], NUM_TOKENS);
            if (!isBitSet(tempFollow, EPS)) {
                break;
            } else {
                clearBit(tempFollow, EPS);
            }
        }
        ele = ele ->next;
    }
    addSetToTable(nonTerminal, tempFollow, ruleNo);
}

void populateParseTable() {
    for(int i = 0; i < NUM_RULES; i++) {
        GrammarEle* ele = getGrammarRule(i);
        NonTerminal head = ele ->symbol ->nonTerminal;
        ele = ele ->next;
        if(ele ->isTerminal && ele ->symbol ->token == EPS) {
            addSetToTable(head, followSet[head], i);
        } else {
            addFirstSetToTable(head, ele, i);
        }
    }
}

int getRuleNoFromTable(NonTerminal nonTerminal, Token token) {
    return parseTable[nonTerminal][token];
}

void freeFirstAndFollowSets() {
    freeSet(firstSet);
    freeSet(followSet);
    free(tempFollow);
    free(doneFirstCalc);
}

void freeParseTable() {
    for(int i = 0; i < NUM_NON_TERMINALS; i++) {
        free(parseTable[i]);
        parseTable[i] = NULL;
    }
    free(parseTable);
    parseTable = NULL;
}

void computeParseTable() {
    initFirstAndFollowSets();
    computeFirstSet();
    computeFollowSet();
    initParseTable();
    populateParseTable();
}

//debug
void printFirstSet() {
    // for(int i = 0; i < NUM_NON_TERMINALS; i++) {
    //     printf("%s: ", getNonTerminalStr(i));
    //     for(int j = 0; j < NUM_TOKENS; j++) {
    //         if (isBitSet(firstSet[i], j)) {
    //             printf("%s ", getTokenStr(j));
    //         }
    //     }
    //     printf("\t|\t");
    //     for(int j = 0; j < NUM_TOKENS; j++) {
    //         if(isBitSet(followSet[i], j)) {
    //             printf("%s ", getTokenStr(j));
    //         }
    //     }
    //     printf("\n");
    // }
}