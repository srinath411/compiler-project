#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "hash_table.h"

int hashFunction(char* str, int numBuckets) {
    int asciiSum = 0;
    for(int i = 0; str[i] != '\0'; i++) {
        asciiSum = (asciiSum + str[i]) % numBuckets;
    }
    return asciiSum;
}

hashEle** initHashTable(char* arr[], int arrLen, int numBuckets) {
    hashEle** buckets = (hashEle**) malloc(numBuckets * (sizeof(hashEle*)));
    for(int i = 0; i < numBuckets; i++) {
        buckets[i] = NULL;
    }
    for (int i = 0; i < arrLen; i++) {
        int bucketNo = hashFunction(arr[i], numBuckets);
        hashEle* ele = (hashEle*) malloc (sizeof(hashEle));
        ele ->str = arr[i];
        ele ->val = i;
        ele ->next = buckets[bucketNo];
        buckets[bucketNo] = ele;
    }
    return buckets;
}

int findEleInTable(hashEle** buckets, int numBuckets, char* str) {
    int bucketNo = hashFunction(str, numBuckets);
    hashEle* head = buckets[bucketNo];
    while(head != NULL) {
        if (strcmp(head ->str, str) == 0) {
            return head ->val;
        }
        head = head ->next;
    }
    return -1;
}

void freeHashTable(hashEle** buckets, int numBuckets) {
    for(int i = 0; i < numBuckets; i++) {
        hashEle* head = buckets[i];
        while(head != NULL) {
            hashEle* temp = head;
            head = head ->next;
            free(temp);
            temp = NULL;
        }
        buckets[i] = NULL;
    }
}

//debug
void printHashTable(hashEle** buckets, int numBuckets) {
    for(int i = 0; i < numBuckets; i++) {
        printf("Bucket %d: ", i);
        hashEle* head = buckets[i];
        while(head != NULL) {
            printf("%s ", head ->str);
            head = head ->next;
        }
        printf("\n");
    }
}