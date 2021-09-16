#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../common/symbols.h"
#include "grammar.h"
#include "../utils/bit_array.h"
#define BLOCK_SIZE 30
#define LINE_SIZE (2 * BLOCK_SIZE + 4)

int** firstSet = NULL;
int** followSet = NULL;
int** parseTable = NULL;

int* tempSet; // Temporary set to store certain first/follow set values
int* doneFirstCalc; // Indicates whether first set calculation is completed or not
int* before; // Previous follow set values, used to compare with current values

/*
 * Initializes and returns First/Follow set
 */
int** initSet() {
    int** set = (int**) malloc (NUM_NON_TERMINALS * sizeof(int*));
    for(int i = 0; i < NUM_NON_TERMINALS; i++) {
        // Use bit array to save memory as we only need 0/1
        set[i] = initBitArr(NUM_TOKENS);
    }
    return set;
}

/*
 * Deallocates memory from First/Follow set
 */
void freeSet(int** set) {
    for(int i = 0; i < NUM_NON_TERMINALS; i++) {
        free(set[i]);
        set[i] = NULL;
    }
    free(set);
    set = NULL;
}

/*
 * Returns 1 if follow set of nonterminal contains token, 0 otherwise
 */
int followSetContains(NonTerminal nonTerminal, Token token) {
    return isBitSet(followSet[nonTerminal], token);
}

/*
 * Initializes First and Follow sets along with some helper structures
 */
void initFirstAndFollowSets() {
    firstSet = initSet();
    followSet = initSet();
    tempSet = initBitArr(NUM_TOKENS);
    before = initBitArr(NUM_TOKENS);
    doneFirstCalc = initBitArr(NUM_NON_TERMINALS);
}

/*
 * Initializes all parse table entries with -1
 */
void initParseTable() {
    parseTable = (int**) malloc(NUM_NON_TERMINALS * sizeof(int*));
    for(int i = 0; i < NUM_NON_TERMINALS; i++) {
        parseTable[i] = (int*) malloc(NUM_TOKENS * sizeof(int));
        for(int j = 0; j < NUM_TOKENS; j++) {
            parseTable[i][j] = -1;
        }
    }
}

/*
 * Computes first set for nonTerminal recursively
 */
void calcFirstSet(NonTerminal nonTerminal) {
    // Exit if completed previously
    if (isBitSet(doneFirstCalc, nonTerminal)) {
        return;
    }
    int firstOccurence = getFirstOccurence(nonTerminal);
    int lastOccurence = getLastOccurence(nonTerminal);
    if (firstOccurence == -1 || lastOccurence < firstOccurence) {
        printf("Error\n");
        return;
    }
    // Assume rules of same non terminal appear consecutively
    for(int i = firstOccurence; i <= lastOccurence; i++) {
        GrammarEle* ele = getGrammarRule(i);
        if (ele == NULL || ele ->next == NULL) {
            printf("Error\n");
            return;
        }
        ele = ele ->next;
        while(ele != NULL) {
            clearBit(firstSet[nonTerminal], EPS);
            if (ele ->isTerminal) {
                // Add token to first set and stop
                setBit(firstSet[nonTerminal], ele ->symbol ->token);
                break;
            } else {
                // Recursively calculate for next nonTerminal and take union
                calcFirstSet(ele ->symbol ->nonTerminal);
                computeBitArrUnion(firstSet[nonTerminal], firstSet[ele ->symbol ->nonTerminal], NUM_TOKENS);

                if (!isBitSet(firstSet[ele ->symbol ->nonTerminal], EPS)) {
                    // If EPS not present, we can stop for this rule
                    break;
                }
            }
            ele = ele ->next;
        }
    }
    // Indicate first set calculation is done for nonTerminal
    setBit(doneFirstCalc, nonTerminal);
}

/*
 * Computes first set for all Non Terminals
 */
void computeFirstSet() {
    for(int i = 0; i < NUM_NON_TERMINALS; i++) {
        calcFirstSet(i);
    }
}

/*
 * Computes follow set for all nonTerminals in RHS of a rule
 * Returns 1 if follow set of any non terminal has changed from previous computation, 0 otherwise
 */
int computeFollowInRule(NonTerminal head, GrammarEle* ele) {
    int setChanged = 0; // Indicates if any follow set has changed
    if (ele == NULL) {
        // Propagate follow set of head in case right most elements require it
        copyBitArr(tempSet, followSet[head], NUM_TOKENS);
        return 0;
    }
    // Moving from right to left in the rule
    setChanged = computeFollowInRule(head, ele ->next);

    if (ele ->isTerminal) {
        // Clear tempSet and add only this token
        clearAllBits(tempSet, NUM_TOKENS);
        setBit(tempSet, ele ->symbol ->token);
        return setChanged;
    } else {
        // Initialize before
        copyBitArr(before, followSet[ele ->symbol ->nonTerminal], NUM_TOKENS);

        // Directly use tempSet instead of traversing the rule
        computeBitArrUnion(followSet[ele ->symbol ->nonTerminal], tempSet, NUM_TOKENS);
        clearBit(followSet[ele ->symbol ->nonTerminal], EPS);

        // Check if follow set has changed
        if (!bitArrsEqual(before, followSet[ele ->symbol ->nonTerminal], NUM_TOKENS)) {
            setChanged = 1;
        }

        // Update tempSet
        if (!isBitSet(firstSet[ele ->symbol ->nonTerminal], EPS)) {
            // Update tempSet to only propagate first of nonTerminal
            copyBitArr(tempSet, firstSet[ele ->symbol ->nonTerminal], NUM_TOKENS);
            clearBit(tempSet, EPS);
        } else {
            // Need to propagate previous values along with first set of nonTerminal
            computeBitArrUnion(tempSet, firstSet[ele ->symbol ->nonTerminal], NUM_TOKENS);
        }
        return setChanged;
    }
}

/*
 * Computes follow sets until stable
 */
void computeFollowSet() {
    // Add dollar to followSet of program
    setBit(followSet[program], DOLLAR);

    int setChanged = 1; // Indicates if any follow set has changed
    while(setChanged) {
        setChanged = 0;
        for(int i = 0; i < NUM_RULES; i++) {
            GrammarEle* ele = getGrammarRule(i);
            NonTerminal head = ele ->symbol ->nonTerminal;
            ele = ele ->next;
            clearAllBits(tempSet, NUM_TOKENS);
            setChanged = setChanged || computeFollowInRule(head, ele);
        }
    }
}

/*
 * Adds ruleNo to parse table for all tokens present in set
 */
void addSetToTable(NonTerminal nonTerminal, int* set, int ruleNo) {
    for(int i = 0; i < NUM_TOKENS; i++) {
        if (isBitSet(set, i)) {
            parseTable[nonTerminal][i] = ruleNo;
        }
    }
}

/*
 * Updates parse table for a particular rule by computing first of RHS
 */
void addFirstSetToTable(NonTerminal nonTerminal, GrammarEle* ele, int ruleNo) {
    clearAllBits(tempSet, NUM_TOKENS);
    while(ele != NULL) {
        if (ele ->isTerminal) {
            setBit(tempSet, ele ->symbol ->token);
            break;
        } else {
            computeBitArrUnion(tempSet, firstSet[ele ->symbol ->nonTerminal], NUM_TOKENS);
            if (!isBitSet(tempSet, EPS)) {
                break;
            } else {
                clearBit(tempSet, EPS);
            }
        }
        ele = ele ->next;
    }
    addSetToTable(nonTerminal, tempSet, ruleNo);
}

/*
 * Fills all columns for a particular nonTerminal with ruleNo
 */
void fillTableEntriesWithRule(NonTerminal nonTerminal, int ruleNo) {
    for(int i = 0; i < NUM_TOKENS; i++) {
        if (parseTable[nonTerminal][i] == -1) {
            parseTable[nonTerminal][i] = ruleNo;
        }
    }
}

/*
 * Populates parse table using first/follow sets calculated
 */
void populateParseTable() {
    for(int i = 0; i < NUM_RULES; i++) {
        GrammarEle* ele = getGrammarRule(i);
        NonTerminal head = ele ->symbol ->nonTerminal;
        ele = ele ->next;
        if(ele ->isTerminal && ele ->symbol ->token == EPS) {
            // Ideally should be follow set, but filling all entries with EPS rule simplifies error handling
            fillTableEntriesWithRule(head, i);
        } else {
            addFirstSetToTable(head, ele, i);
        }
    }
}

/*
 * Returns ruleNo present in parse table
 */
int getRuleNoFromTable(NonTerminal nonTerminal, Token token) {
    return parseTable[nonTerminal][token];
}

/*
 * Deallocates memory from first, follow sets and other internal structures
 */
void freeFirstAndFollowSets() {
    freeSet(firstSet);
    freeSet(followSet);
    free(tempSet);
    free(doneFirstCalc);
}

/*
 * Deallocates memory from parse table
 */
void freeParseTable() {
    for(int i = 0; i < NUM_NON_TERMINALS; i++) {
        free(parseTable[i]);
        parseTable[i] = NULL;
    }
    free(parseTable);
    parseTable = NULL;
}

/*
 * Creates first, follow sets and populates parse table
 */
void computeParseTable() {
    initFirstAndFollowSets();
    computeFirstSet();
    computeFollowSet();
    initParseTable();
    populateParseTable();
}

/*
 * Removes tokens present in the line
 */
void resetLine(char line[]) {
    int i1 = BLOCK_SIZE + 1, i2 = LINE_SIZE  - 1;
    for(int k = 0; k < LINE_SIZE; k++) {
        line[k] = ' ';
    }
    line[i1] = '|';
    line[i2] = '\0';
}

/*
 * Prints characters to act as row separator
 */
void printRowSeparator() {
    for(int k = 0; k < BLOCK_SIZE + 1; k++) {
        printf("_");
    }
    printf("|");
    for(int k = 0; k < BLOCK_SIZE + 2; k++) {
        printf("_");
    }
    printf("|");
    for(int k = 0; k < BLOCK_SIZE + 1; k++) {
        printf("_");
    }
    printf("\n");
}

/*
 * Print first and follow sets for the given non terminal
 */
void printReqdTokens(NonTerminal nonTerminal) {
    char line[LINE_SIZE];
    resetLine(line);
    int currSize, printedNonTerminal = 0;
    int i = 0, j = 0;
    while(i < NUM_TOKENS || j < NUM_TOKENS) {
        currSize = BLOCK_SIZE;
        resetLine(line);
        while(i < NUM_TOKENS) {
            if (isBitSet(firstSet[nonTerminal], i)) {
                char* token = getTokenStr(i);
                int tokenLen = strlen(token);
                if (tokenLen <= currSize) {
                    int linePos = BLOCK_SIZE - currSize;
                    strncpy(line + linePos, token, tokenLen);
                    currSize = currSize - tokenLen - 1;
                } else {
                    break;
                }
            }
            i++;
        }
        currSize = BLOCK_SIZE;
        while(j < NUM_TOKENS) {
            if (isBitSet(followSet[nonTerminal], j)) {
                char* token = getTokenStr(j);
                int tokenLen = strlen(token);
                if (tokenLen <= currSize) {
                    int linePos = LINE_SIZE - 1 - currSize;
                    strncpy(line + linePos, token, tokenLen);
                    currSize = currSize - tokenLen - 1;
                } else {
                    break;
                }
            }
            j++;
        }
        if (printedNonTerminal) {
            printf("%-30s | %s\n", "", line);
        } else {
            printf("%-30s | %s\n", getNonTerminalStr(nonTerminal), line);
            printedNonTerminal = 1;
        }
    }
    printRowSeparator();
}

/*
 * Print the calculated first and follow sets for each non terminal
 */
void printFirstAndFollowSets() {
    printf("%-30s | %-30s | %s\n", "NON TERMINAL", "FIRST SET", "FOLLOW SET");
    printRowSeparator();
    for(int i = 0; i < NUM_NON_TERMINALS; i++) {
        printReqdTokens(i);
    }
}