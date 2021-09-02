#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../lexer/tokenStream.h"
#include "grammar.h"
#include "parse_tree.h"
#include "parse_table.h"
#include "../utils/stack.h"

void addGrammarEleToStack(Stack* stack, TreeNode* head, GrammarEle* ele) {
    if (ele == NULL) {
        return;
    }
    addGrammarEleToStack(stack, head, ele ->next);
    TreeNode* node = NULL;
    if (ele ->isTerminal) {
        node = createTreeNode(ele ->symbol ->token, leafNodeType);
    } else {
        node = createTreeNode(ele ->symbol ->nonTerminal, parentNodeType);
    }
    addChildToNode(head, node);
    push(stack, node);
}

void parseTokens() {
    TreeNode* root = createTreeNode(program, parentNodeType);
    Stack* stack = createNewStack();
    tokenEle* streamEle = getTokenFromStream();
    push(stack, root);
    while(stack ->stackSize != 0) {
        TreeNode* node = pop(stack);
        node ->lineNo = streamEle ->lineNo;
        if (node ->nodeType == leafNodeType) {
            Token token = node ->info ->leaf ->token;
            if (token != EPS) {
                if (token == streamEle ->token) {
                    node ->info ->leaf ->lexeme = getCopyOfLexeme(streamEle ->lexeme, token);
                } else {
                    printf("Error!!! Line no: %d, Expected: %s Got: %s\n", 
                        streamEle ->lineNo, getTokenStr(token), getTokenStr(streamEle ->token));
                    return;
                }
                free(streamEle);
                streamEle = getTokenFromStream();
            }
        } else {
            NonTerminal nonTerminal = node ->info ->parent ->nonTerminal;
            int ruleNo = getRuleNoFromTable(nonTerminal, streamEle ->token);
            if (ruleNo == -1) {
                printf("Rule Error!!! Line no: %d, NonTerminal: %s Token: %s\n", 
                    streamEle ->lineNo, getNonTerminalStr(nonTerminal), getTokenStr(streamEle ->token));
                return;
            }
            GrammarEle* ele = getGrammarRule(ruleNo);
            ele = ele ->next;
            addGrammarEleToStack(stack, node, ele);
        }
    }
    printParseTree(root);
    freeParseTree(root);
    freeStack(stack);
    stack = NULL;
}