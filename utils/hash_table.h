#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef struct HashEle {
    char* str;
    int val;
    struct HashEle* next;
} HashEle;

typedef struct HashTable {
    HashEle** buckets;
    int numBuckets;
} HashTable;

HashTable* initHashTable(char* arr[], int arrLen);
int findEleInTable(HashTable* hashTable, char* str);
void freeHashTable(HashTable* hashTable);
void printHashTable(HashTable* hashTable);

#endif