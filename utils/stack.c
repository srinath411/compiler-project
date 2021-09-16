#include<stdio.h>
#include<stdlib.h>
#include "stack.h"
#define ARR_SIZE 50

/*
 * Initializes and returns a new stack
 */
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

/*
 * Adds new element to the top of stack
 */
void push(Stack* stack, TreeNode* ele) {
    if (stack ->stackSize + 1 == stack ->arrSize) {
        printf("Stack Error\n");
        return;
    }
    stack ->stackSize++;
    stack ->arr[++stack ->top] = ele;
}

/*
 * Removes element present at the top of stack and returns the element
 * Returns Null if no element present
 */
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

/*
 * Returns element present at the top of stack without removing
 * Returns Null if no element present
 */
TreeNode* peek(Stack* stack) {
    if (stack ->stackSize == 0) {
        return NULL;
    }
    return stack ->arr[stack ->top];
}

/*
 * Frees memory allocated to stack
 */
void freeStack(Stack* stack) {
    if (stack != NULL && stack ->arr != NULL) {
        free(stack ->arr);
        stack ->arr = NULL;
    }
    free(stack);
}