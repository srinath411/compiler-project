#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef struct HashEle {
    char* key;
    void* val;
    struct HashEle* next;
} HashEle;

typedef struct HashTable {
    HashEle** buckets;
    int numBuckets;
} HashTable;

HashTable* initHashTable(int numElements);
void hashEleInTable(HashTable* hashTable, char* key, void* val);
void* findEleInTable(HashTable* hashTable, char* str);
void freeHashTable(HashTable* hashTable);
void printHashTable(HashTable* hashTable);

#endif