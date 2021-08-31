#include<stdio.h>
#include<stdlib.h>
#include "../common/tokens.h"
#include "../common/grammar.h"
#define INT_SIZE ((int)sizeof(int) * 8)
#define SET_SIZE (NUM_TOKENS / INT_SIZE) + 1
#define DONE_SIZE (NUM_NON_TERMINALS / INT_SIZE) + 1

int** firstSet = NULL;
int** followSet = NULL;

int parseTable[NUM_NON_TERMINALS][NUM_TOKENS];

int tempFollow[SET_SIZE];
int doneFirstCalc[DONE_SIZE];

int** initSet() {
    int** set = (int**) malloc (NUM_NON_TERMINALS * sizeof(int*));
    for(int i = 0; i < NUM_NON_TERMINALS; i++) {
        set[i] = (int*) malloc (SET_SIZE * sizeof(int));
        for(int j = 0; j < SET_SIZE; j++) {
            set[i][j] = 0;
        }
    }
    return set;
}

void freeSet(int** set) {
    for(int i = 0; i < NUM_NON_TERMINALS; i++) {
        free(set[i]);
        set[i] = NULL;
    }
    set = NULL;
}

void setBit(int* set, int pos) {
    int bitPos = pos % INT_SIZE;
    int arrPos = pos / INT_SIZE;
    set[arrPos] |= 1 << bitPos;
}

void clearBit(int* set, int pos) {
    int bitPos = pos % INT_SIZE;
    int arrPos = pos / INT_SIZE;
    set[arrPos] &= ~(1 << bitPos);   
}

int isBitSet(int* set, int pos) {
    int bitPos = pos % INT_SIZE;
    int arrPos = pos / INT_SIZE;
    return set[arrPos] & (1 << bitPos);
}

void computeSetUnion(int* set1, int* set2) {
    for(int i = 0; i < SET_SIZE; i++) {
        set1[i] |= set2[i];
    }
}

void markFirstCalcDone(NonTerminal nonTerminal) {
    setBit(doneFirstCalc, nonTerminal);
}

int firstCalcDone(NonTerminal nonTerminal) {
    return isBitSet(doneFirstCalc, nonTerminal);
}

void addTokenToSet(int* set, Token token) {
    setBit(set, token);
}

void clearTokenFromSet(int* set, Token token) {
    clearBit(set, token);
}

int setContains(int* set, Token token) {
    return isBitSet(set, token);
}

void initFirstAndFollowSets() {
    firstSet = initSet();
    followSet = initSet();
}

void initParseTable() {
    for(int i = 0; i < NUM_NON_TERMINALS; i++) {
        for(int j = 0; j < NUM_TOKENS; j++) {
            parseTable[i][j] = -1;
        }
    }
}

void calcFirstSet(NonTerminal nonTerminal) {
    if (firstCalcDone(nonTerminal)) {
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
                addTokenToSet(firstSet[nonTerminal], ele ->symbol);
                if (ele ->symbol == EPS) {
                    isEpsilonPresent = 1;
                }
                break;
            } else {
                calcFirstSet(ele ->symbol);
                computeSetUnion(firstSet[nonTerminal], firstSet[ele ->symbol]);
                if (!setContains(firstSet[ele ->symbol], EPS)) {
                    break;
                } else if (ele ->next == NULL) {
                    isEpsilonPresent = 1;
                }
            }
            clearTokenFromSet(firstSet[nonTerminal], EPS);
            ele = ele ->next;
        }
    }
    if (isEpsilonPresent) {
        addTokenToSet(firstSet[nonTerminal], EPS);
    }
    markFirstCalcDone(nonTerminal);
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
        for (int i = 0; i < SET_SIZE; i++) {
            tempFollow[i] = 0;
        }
        addTokenToSet(tempFollow, ele ->symbol);
        return setChanged;
    } else {
        int before[SET_SIZE];
        for (int i = 0; i < SET_SIZE; i++) {
            before[i] = followSet[ele ->symbol][i];
        }
        if (ele ->next == NULL) {
            computeSetUnion(followSet[ele ->symbol], followSet[head]);
            if (setContains(firstSet[ele ->symbol], EPS)) {
                for(int i = 0; i < SET_SIZE; i++) {
                    tempFollow[i] = followSet[head][i];
                }
            }
        } else if (ele ->next ->isTerminal) {
            addTokenToSet(followSet[ele ->symbol], ele ->next ->symbol);
        } else {
            computeSetUnion(followSet[ele ->symbol], firstSet[ele ->next ->symbol]);
            if (setContains(followSet[ele ->symbol], EPS)) {
                computeSetUnion(followSet[ele ->symbol], tempFollow);
                clearTokenFromSet(followSet[ele ->symbol], EPS);
            }
        }
        for(int i = 0; i < SET_SIZE; i++) {
            if (before[i] != followSet[ele ->symbol][i]) {
                setChanged = 1;
                break;
            }
        }
        if (!setContains(firstSet[ele ->symbol], EPS)) {
            for(int i = 0; i < SET_SIZE; i++) {
                tempFollow[i] = firstSet[ele ->symbol][i];
            }
            clearTokenFromSet(tempFollow, EPS);
        } else {
            computeSetUnion(tempFollow, firstSet[ele ->symbol]);
        }
        return setChanged;
    }
}

void computeFollowSet() {
    addTokenToSet(followSet[program], DOLLAR);
    int setChanged = 1;
    while(setChanged) {
        setChanged = 0;
        for(int i = 0; i < NUM_RULES; i++) {
            GrammarEle* ele = getGrammarRule(i);
            if (ele == NULL || ele ->next == NULL) {
                printf("Error\n");
                return;
            }
            NonTerminal head = ele ->symbol;
            ele = ele ->next;
            for (int i = 0; i < SET_SIZE; i++) {
                tempFollow[i] = 0;
            }
            setChanged = setChanged || computeFollowInRule(head, ele);
        }
    }
}

void addSetToTable(NonTerminal nonTerminal, int* set, int ruleNo) {
    for(int i = 0; i < NUM_TOKENS; i++) {
        if (setContains(set, i)) {
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
    for(int i = 0; i < SET_SIZE; i++) {
        tempFollow[i] = 0;
    }
    while(ele != NULL) {
        if (ele ->isTerminal) {
            addTokenToSet(tempFollow, ele ->symbol);
            break;
        } else {
            computeSetUnion(tempFollow, firstSet[ele ->symbol]);
            if (!setContains(tempFollow, EPS)) {
                break;
            } else {
                clearTokenFromSet(tempFollow, EPS);
            }
        }
        ele = ele ->next;
    }
    addSetToTable(nonTerminal, tempFollow, ruleNo);
}

void populateParseTable() {
    for(int i = 0; i < NUM_RULES; i++) {
        GrammarEle* ele = getGrammarRule(i);
        NonTerminal head = ele ->symbol;
        ele = ele ->next;
        if(ele ->isTerminal && ele ->symbol == EPS) {
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
    //         if (setContains(firstSet[i], j)) {
    //             printf("%s ", getTokenStr(j));
    //         }
    //     }
    //     printf("\t|\t");
    //     for(int j = 0; j < NUM_TOKENS; j++) {
    //         if(setContains(followSet[i], j)) {
    //             printf("%s ", getTokenStr(j));
    //         }
    //     }
    //     printf("\n");
    // }
}