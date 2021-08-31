#include <stdlib.h>
#include<string.h>
#include "../common/tokens.h"
#include "keywords.h"
#include "../utils/hash_table.h"
#define NUM_KEYWORDS 24
#define NUM_BUCKETS 5

hashEle** hashTable = NULL;

char* keywordStrings[] = {"with", "parameters", "end", "while", "int", "real", "type", "_main", "global", "parameter","list", "input", 
"output", "endwhile", "if", "then", "endif", "read", "write", "return", "call", "record", "endrecord", "else"};

Token keywordTokens[] = {WITH, PARAMETERS, END, WHILE, INT, REAL, TYPE, MAIN, GLOBAL, PARAMETER, LIST, INPUT, OUTPUT, END_WHILE, IF, 
THEN, END_IF, READ, WRITE, RETURN, CALL, RECORD, END_RECORD, ELSE};

void initKeywordTable() {
    hashTable = initHashTable(keywordStrings, NUM_KEYWORDS, NUM_BUCKETS);
}

Token getKeywordIfPresent(char* lexeme) {
    int index = findEleInTable(hashTable, NUM_BUCKETS, lexeme);
    return (index == -1) ? -1 : keywordTokens[index];
}

void freeKeywordTable() {
    freeHashTable(hashTable, NUM_BUCKETS);
    hashTable = NULL;
}

//debug
void printKeywordTable() {
    printHashTable(hashTable, NUM_BUCKETS);
}