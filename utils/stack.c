#include<stdio.h>
#include<stdlib.h>
#include "stack.h"
#define ARR_SIZE 50

Stack* createNewStack() {
    Stack* stack = (Stack*) malloc (sizeof(Stack));
    stack ->top = -1;
    stack ->stackSize = 0;
    stack ->arrSize = ARR_SIZE;
    stack ->arr = (TreeNode**) malloc(ARR_SIZE * sizeof(TreeNode*));
    for(int i = 0; i < ARR_SIZE; i++) {
        stack ->arr[i] = NULL;
    }
    return stack;
}

void push(Stack* stack, TreeNode* ele) {
    if (stack ->stackSize + 1 == stack ->arrSize) {
        printf("Stack Error\n");
        return;
    }
    stack ->stackSize++;
    stack ->arr[++stack ->top] = ele;
}

TreeNode* pop(Stack* stack) {
    if (stack ->stackSize == 0) {
        return NULL;
    }
    stack ->stackSize--;
    TreeNode* ele = stack ->arr[stack ->top];
    stack ->arr[stack ->top] = NULL;
    stack ->top--;
    return ele;
}

TreeNode* peek(Stack* stack) {
    if (stack ->stackSize == 0) {
        return NULL;
    }
    return stack ->arr[stack ->top];
}

void freeStack(Stack* stack) {
    if (stack != NULL && stack ->arr != NULL) {
        free(stack ->arr);
        stack ->arr = NULL;
    }
    free(stack);
}