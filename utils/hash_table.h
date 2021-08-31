#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef struct hashEle {
    char* str;
    int val;
    struct hashEle* next;
} hashEle;

hashEle** initHashTable(char* arr[], int arrLen, int numBuckets);
int findEleInTable(hashEle** buckets, int numBuckets, char* str);
void freeHashTable(hashEle** buckets, int numBuckets);
void printHashTable(hashEle** buckets, int numBuckets);

#endif