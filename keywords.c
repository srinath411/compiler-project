#include <stdlib.h>
#include<string.h>
#include "tokens.h"
#include "keywords.h"
#define NUM_KEYWORDS 24
#define NUM_BUCKETS 5

typedef struct keywordEntry {
    char *str;
    Token token;
    struct keywordEntry *next;
} keywordEntry;

char* keywordStrings[] = {"with", "parameters", "end", "while", "int", "real", "type", "_main", "global", "parameter","list", "input", 
"output", "endwhile", "if", "then", "endif", "read", "write", "return", "call", "record", "endrecord", "else"};

Token keywordTokens[] = {WITH, PARAMETERS, END, WHILE, INT, REAL, TYPE, MAIN, GLOBAL, PARAMETER, LIST, INPUT, OUTPUT, END_WHILE, IF, 
THEN, END_IF, READ, WRITE, RETURN, CALL, RECORD, END_RECORD, ELSE};

keywordEntry* keywordTable[NUM_BUCKETS];

int hashFunction(char* str) {
    int asciiSum = 0;
    int len = strlen(str);
    for(int i = 0; i < len; i++) {
        asciiSum += str[i];
    }
    return asciiSum % NUM_BUCKETS;
}

keywordEntry* makeKeywordEntry(char* str, Token token) {
    keywordEntry* entry = (keywordEntry*) malloc (sizeof(keywordEntry));
    entry ->str = str;
    entry ->token = token;
    entry ->next = NULL;
    return entry;
}

void insertEntryInTable(keywordEntry* entry, int index) {
    keywordEntry* head = keywordTable[index];
    entry ->next = head;
    keywordTable[index] = entry;
}

void initKeywordTable() {
    for(int i = 0; i < NUM_BUCKETS; i++) {
        keywordTable[i] = NULL;
    }
    for(int i = 0; i < NUM_KEYWORDS; i++) {
        int bucketNo = hashFunction(keywordStrings[i]);
        keywordEntry* entry = makeKeywordEntry(keywordStrings[i], keywordTokens[i]);
        insertEntryInTable(entry, bucketNo);
    }
}

Token getKeywordIfPresent(char* lexeme) {
    int bucketNo = hashFunction(lexeme);
    keywordEntry* head = keywordTable[bucketNo];
    while((head != NULL) && (strcmp(head ->str, lexeme) != 0)) {
        head = head ->next;
    }
    if (head == NULL) {
        return -1;
    } else {
        return head ->token;
    }
}

void freeKeywordBucket(int bucketNo) {
    keywordEntry* head = keywordTable[bucketNo];
    while(head != NULL) {
        keywordEntry* temp = head;
        head = head ->next;
        free(temp);
    }
}

void freeKeywordTable() {
    for(int i = 0; i < NUM_BUCKETS; i++) {
        freeKeywordBucket(i);
    }
}
