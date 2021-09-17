#include <stdlib.h>
#include "../common/symbols.h"
#include "../utils/hash_table.h"
#define NUM_KEYWORDS 24
#define NUM_BUCKETS 5

HashTable* hashTable = NULL;

char* keywordStrings[] = {"with", "parameters", "end", "while", "int", "real", "type", "_main", "global", "parameter","list", "input", 
"output", "endwhile", "if", "then", "endif", "read", "write", "return", "call", "record", "endrecord", "else"};

Token keywordTokens[] = {WITH, PARAMETERS, END, WHILE, INT, REAL, TYPE, MAIN, GLOBAL, PARAMETER, LIST, INPUT, OUTPUT, END_WHILE, IF, 
THEN, END_IF, READ, WRITE, RETURN, CALL, RECORD, END_RECORD, ELSE};

/*
 * Initializes a hash table for keywords
 */
void initKeywordTable() {
    hashTable = initHashTable(NUM_KEYWORDS);
    for(int i = 0; i < NUM_KEYWORDS; i++) {
        hashEleInTable(hashTable, keywordStrings[i], &keywordTokens[i]);
    }
}

/*
 * Keyword string --> Token
 */
Token getKeywordIfPresent(char* lexeme) {
    Token* tokenPtr = (Token*)findEleInTable(hashTable, lexeme);
    return (tokenPtr == NULL) ? -1 : *tokenPtr;
}

/*
 * Deallocates memory from keyword table
 */
void freeKeywordTable() {
    freeHashTable(hashTable);
    hashTable = NULL;
}

//debug
void printKeywordTable() {
    printHashTable(hashTable);
}