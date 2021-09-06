#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "hash_table.h"

int getNumBuckets(int arrLen) {
    arrLen--;
    int numBuckets = 2;
    while(arrLen >>= 1) {
        numBuckets <<= 1;
    }
    return numBuckets;
}

int hashFunction(char* str, int numBuckets) {
    numBuckets--;
    int hash = 5381;
    for(int i = 0; str[i] != '\0'; i++) {
        hash = (((((hash << 5) & numBuckets) + hash) & numBuckets) + str[i]) & numBuckets;
    }
    return hash;
}

HashTable* initHashTable(char* arr[], int arrLen) {
    int numBuckets = getNumBuckets(arrLen);
    HashEle** buckets = (HashEle**) malloc(numBuckets * (sizeof(HashEle*)));
    for(int i = 0; i < numBuckets; i++) {
        buckets[i] = NULL;
    }
    for (int i = 0; i < arrLen; i++) {
        int bucketNo = hashFunction(arr[i], numBuckets);
        HashEle* ele = (HashEle*) malloc (sizeof(HashEle));
        ele ->str = arr[i];
        ele ->val = i;
        ele ->next = buckets[bucketNo];
        buckets[bucketNo] = ele;
    }
    HashTable* hashTable = (HashTable*) malloc(sizeof(HashTable));
    hashTable ->buckets = buckets;
    hashTable ->numBuckets = numBuckets;
    return hashTable;
}

int findEleInTable(HashTable* hashTable, char* str) {
    int bucketNo = hashFunction(str, hashTable ->numBuckets);
    HashEle* head = hashTable ->buckets[bucketNo];
    while(head != NULL) {
        if (strcmp(head ->str, str) == 0) {
            return head ->val;
        }
        head = head ->next;
    }
    return -1;
}

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
            printf("%s ", head ->str);
            head = head ->next;
        }
        printf("\n");
    }
}