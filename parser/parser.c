#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../common/error_handler.h"
#include "../lexer/token_stream.h"
#include "grammar.h"
#include "parse_tree.h"
#include "parse_table.h"
#include "../utils/stack.h"

TreeNode* root = NULL;

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
    root = createTreeNode(program, parentNodeType);
    Stack* stack = createNewStack();
    TokenEle* streamEle = getTokenFromStream();
    push(stack, root);
    while(stack ->stackSize != 0 && streamEle != NULL) {
        TreeNode* node = pop(stack);
        node ->lineNo = streamEle ->lineNo;
        if (node ->nodeType == leafNodeType) {
            Token token = node ->info ->leaf ->token;
            if (token != EPS) {
                if (token == streamEle ->token) {
                    node ->info ->leaf ->lexeme = streamEle ->lexeme;
                } else {
                    TreeNode* tempNode = peek(stack);
                    TokenEle* tempEle = streamEle;
                    if (tempNode != NULL) {
                        if (tempNode ->nodeType == leafNodeType) {
                            while(streamEle != NULL) {
                                if (token == streamEle ->token) {
                                    if (!tempEle ->hasError) {
                                        printParsingError(streamEle ->lineNo, 4, getTokenStr(token), getTokenStr(streamEle ->token));
                                    }
                                    push(stack, node);
                                    break;
                                } else if (tempNode ->info ->leaf ->token == streamEle ->token) {
                                    if (!tempEle ->hasError) {
                                        printParsingError(streamEle ->lineNo, 5, getTokenStr(token), getTokenStr(streamEle ->token));
                                    }
                                    break;
                                }
                                streamEle = getTokenFromStream();
                            }
                            continue;
                        } else {
                            while(streamEle != NULL) {
                                if (getRuleNoFromTable(tempNode ->info ->parent ->nonTerminal, streamEle ->token) != -1) {
                                    if (!tempEle ->hasError) {
                                        printParsingError(streamEle ->lineNo, 5, getTokenStr(token), getTokenStr(streamEle ->token));
                                    }
                                    break;
                                } else if (token == streamEle ->token) {
                                    if (!tempEle ->hasError) {
                                        printParsingError(streamEle ->lineNo, 4, getTokenStr(token), getTokenStr(streamEle ->token));
                                    }
                                    push(stack, node);
                                    break;
                                }
                                streamEle = getTokenFromStream();
                            }
                            continue;
                        }
                    } else {
                        while(streamEle != NULL) {
                            if (token == streamEle ->token) {
                                if (!tempEle ->hasError) {
                                    printParsingError(streamEle ->lineNo, 4, getTokenStr(token), getTokenStr(streamEle ->token));
                                }
                                push(stack, node);
                                break;
                            }
                            streamEle = getTokenFromStream();
                        }
                        continue;
                    }
                }
                streamEle = getTokenFromStream();
            }
        } else {
            NonTerminal nonTerminal = node ->info ->parent ->nonTerminal;
            int ruleNo = getRuleNoFromTable(nonTerminal, streamEle ->token);
            if (ruleNo == -1) {
                if (!streamEle ->hasError) {
                    printParsingError(streamEle ->lineNo, 4, getTokenStr(nonTerminal), getTokenStr(streamEle ->token));
                }
                TreeNode* epsNode = createTreeNode(EPS, leafNodeType);
                epsNode ->lineNo = streamEle ->lineNo;
                addChildToNode(node, epsNode);
                while(streamEle != NULL && !followSetContains(nonTerminal, streamEle ->token)) {
                    streamEle = getTokenFromStream();
                }
                continue;
            }
            GrammarEle* ele = getGrammarRule(ruleNo);
            ele = ele ->next;
            addGrammarEleToStack(stack, node, ele);
        }
    }
    while(stack ->stackSize != 0) {
        TreeNode* node = pop(stack);
        if (node ->nodeType == leafNodeType) {
            printf("Missed %s\n", getTokenStr(node ->info ->leaf ->token));
        } else {
            TreeNode* epsNode = createTreeNode(EPS, leafNodeType);
            epsNode ->lineNo = streamEle ->lineNo;
            addChildToNode(node, epsNode);
        }
    }
    freeStack(stack);
    stack = NULL;
}

void printParseTree() {
    printTree(root);
}

void freeParseTree() {
    freeTree(root);
    root = NULL;
}