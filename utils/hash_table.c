#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "hash_table.h"
#define DEFAULT_NUM_BUCKETS 128

/*
 * Returns number of buckets in hash table
 * Number of buckets = lowest power of 2 greater than arrLen
 */
int getNumBuckets(int arrLen) {
    arrLen--;
    int numBuckets = 2;
    while(arrLen >>= 1) {
        numBuckets <<= 1;
    }
    return numBuckets;
}

/*
 * Simple hash function based on djb2 algorithm
 */
int hashFunction(char* key, int numBuckets) {
    // if y is a power of 2, x % y is the same as x & (y - 1)
    numBuckets--;
    int hash = 5381;
    for(int i = 0; key[i] != '\0'; i++) {
        hash = (((((hash << 5) & numBuckets) + hash) & numBuckets) + key[i]) & numBuckets;
    }
    return hash;
}

/*
 * Creates new hash table
 * Hashes elements of arr to their corresponding array indices
 * Returns hash table
 */
HashTable* initHashTable(int numElements) {
    // Initialize hash table
    int numBuckets = numElements == 0 ? DEFAULT_NUM_BUCKETS : getNumBuckets(numElements);
    HashEle** buckets = (HashEle**) malloc(numBuckets * (sizeof(HashEle*)));
    for(int i = 0; i < numBuckets; i++) {
        buckets[i] = NULL;
    }
    HashTable* hashTable = (HashTable*) malloc(sizeof(HashTable));
    hashTable ->buckets = buckets;
    hashTable ->numBuckets = numBuckets;
    return hashTable;
}

void hashEleInTable(HashTable* hashTable, char* key, void* val) {
    HashEle* ele = (HashEle*) malloc (sizeof(HashEle));
    ele ->key = key;
    ele ->val = val;
    int bucketNo = hashFunction(key, hashTable ->numBuckets);
    ele ->next = hashTable ->buckets[bucketNo];
    hashTable ->buckets[bucketNo] = ele;
}

/*
 * Returns index of key in original array
 * Return -1 if key not present
 */
void* findEleInTable(HashTable* hashTable, char* key) {
    int bucketNo = hashFunction(key, hashTable ->numBuckets);
    HashEle* head = hashTable ->buckets[bucketNo];
    while(head != NULL) {
        if (strcmp(head ->key, key) == 0) {
            return head ->val;
        }
        head = head ->next;
    }
    return NULL;
}

/*
 * Deallocates memory from hash table
 */
void freeHashTable(HashTable* hashTable) {
    for(int i = 0; i < hashTable ->numBuckets; i++) {
        HashEle* head = hashTable ->buckets[i];
        while(head != NULL) {
            HashEle* temp = head;
            head = head ->next;
            free(temp);
            temp = NULL;
        }
        hashTable ->buckets[i] = NULL;
    }
    hashTable ->buckets = NULL;
    free(hashTable);
}

//debug
void printHashTable(HashTable* hashTable) {
    for(int i = 0; i < hashTable ->numBuckets; i++) {
        printf("Bucket %d: ", i);
        HashEle* head = hashTable ->buckets[i];
        while(head != NULL) {
            printf("%s ", head ->key);
            head = head ->next;
        }
        printf("\n");
    }
}