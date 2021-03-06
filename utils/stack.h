#ifndef STACK_H
#define STACK_H

#include "../parser/parse_tree.h"

typedef struct Stack {
    int stackSize;
    int arrSize;
    int top;
    TreeNode** arr;
} Stack;

Stack* createNewStack();
void push(Stack* stack, TreeNode* ele);
TreeNode* pop(Stack* stack);
TreeNode* peek(Stack* stack);
void freeStack(Stack* stack);

#endif