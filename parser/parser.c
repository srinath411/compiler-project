#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "../common/error_handler.h"
#include "../lexer/token_stream.h"
#include "grammar.h"
#include "parse_tree.h"
#include "parse_table.h"
#include "../utils/stack.h"

TreeNode* root = NULL; // The root of the parse tree
int lastLineNo = 0; // Useful when some end tokens are missing

/*
 * Pushes the symbols of a rule onto the stack
 */
void addGrammarEleToStack(Stack* stack, TreeNode* head, GrammarEle* ele) {
    if (ele == NULL) {
        return;
    }
    // Push right to left so that top of stack has leftmost symbol
    addGrammarEleToStack(stack, head, ele ->next);

    TreeNode* node = NULL;
    if (ele ->isTerminal) {
        node = createTreeNode(ele ->symbol ->token, leafNodeType);
    } else {
        node = createTreeNode(ele ->symbol ->nonTerminal, parentNodeType);
    }
    // Establish parent - child relationship here itself, would be difficult otherwise
    addChildToNode(head, node);

    push(stack, node);
}

/*
 * Parses the tokens from TokenStream and builds a parse tree
 * Reports all syntax errors in this phase
 */
void parseTokens() {

    // Start with the program nonterminal
    root = createTreeNode(program, parentNodeType);
    Stack* stack = createNewStack();
    TokenEle* streamEle = getTokenFromStream();
    push(stack, root);

    while(stack ->stackSize != 0 && streamEle != NULL) {
        TreeNode* node = pop(stack);
        node ->lineNo = lastLineNo = streamEle ->lineNo;
        if (node ->nodeType == leafNodeType) {
            Token token = node ->info ->leaf ->token;
            if (token != EPS) {
                if (token == streamEle ->token) {
                    node ->info ->leaf ->lexeme = streamEle ->lexeme;
                    streamEle = getTokenFromStream();
                } else {
                    TreeNode* tempNode = peek(stack); // Useful for specific error detections
                    TokenEle* tempEle = streamEle; // Good to store current streamEle in lexical error cases
                    if (tempNode != NULL) {
                        if (tempNode ->nodeType == leafNodeType) {
                            while(streamEle != NULL) {
                                if (token == streamEle ->token) {
                                    // The required token popped from stack was found at a later stage
                                    // This means previous tokens were garbage
                                    if (!tempEle ->hasError) {
                                        printSyntaxError(streamEle ->lineNo, 4, getTokenStr(token), getTokenStr(tempEle ->token));
                                    }
                                    push(stack, node);
                                    break;
                                } else if (tempNode ->info ->leaf ->token == streamEle ->token) {
                                    // The next token on the stack was found
                                    // The popped token is assumed to have been skipped
                                    if (!tempEle ->hasError) {
                                        printSyntaxError(streamEle ->lineNo, 5, getTokenStr(token), getTokenStr(tempEle ->token));
                                    }
                                    break;
                                }
                                streamEle = getTokenFromStream();
                            }
                        } else {
                            while(streamEle != NULL) {
                                if (getRuleNoFromTable(tempNode ->info ->parent ->nonTerminal, streamEle ->token) != -1) {
                                    // A token belonging to the first set of the next non-terminal on the stack was found
                                    // The popped token is assumed to have been skipped
                                    if (!tempEle ->hasError) {
                                        printSyntaxError(streamEle ->lineNo, 5, getTokenStr(token), getTokenStr(tempEle ->token));
                                    }
                                    break;
                                } else if (token == streamEle ->token) {
                                    // The required token popped from stack was found at a later stage
                                    // This means previous tokens were garbage
                                    if (!tempEle ->hasError) {
                                        printSyntaxError(streamEle ->lineNo, 4, getTokenStr(token), getTokenStr(tempEle ->token));
                                    }
                                    push(stack, node);
                                    break;
                                }
                                streamEle = getTokenFromStream();
                            }
                        }
                    } else {
                        while(streamEle != NULL) {
                            // Straightforward panic mode
                            if (token == streamEle ->token) {
                                // The required token popped from stack was found at a later stage
                                // This means previous tokens were garbage
                                if (!tempEle ->hasError) {
                                    printSyntaxError(streamEle ->lineNo, 4, getTokenStr(token), getTokenStr(tempEle ->token));
                                }
                                push(stack, node);
                                break;
                            }
                            streamEle = getTokenFromStream();
                        }
                    }
                }
            }
        } else {
            NonTerminal nonTerminal = node ->info ->parent ->nonTerminal;
            int ruleNo = getRuleNoFromTable(nonTerminal, streamEle ->token);
            if (ruleNo == -1) {
                if (streamEle ->token == DOLLAR) {
                    push(stack, node);
                    break;
                } else if (!streamEle ->hasError) {
                    printSyntaxError(streamEle ->lineNo, 4, getNonTerminalStr(nonTerminal), getTokenStr(streamEle ->token));
                }
                // Force non-terminal to have epsilon as child
                TreeNode* epsNode = createTreeNode(EPS, leafNodeType);
                epsNode ->lineNo = lastLineNo = streamEle ->lineNo;
                addChildToNode(node, epsNode);

                // Straightforward panic mode
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
    // Handles cases where input code terminates prematurely
    while(stack ->stackSize != 0) {
        TreeNode* node = pop(stack);
        if (node ->nodeType == leafNodeType) {
            printSyntaxError(streamEle ->lineNo, 5, getTokenStr(node ->info ->leaf ->token), getTokenStr(DOLLAR));
        } else {
            // Force non-terminal to have epsilon as child
            printSyntaxError(streamEle ->lineNo, 5, getNonTerminalStr(node ->info ->parent ->nonTerminal), getTokenStr(DOLLAR));
            TreeNode* epsNode = createTreeNode(EPS, leafNodeType);
            epsNode ->lineNo = lastLineNo;
            addChildToNode(node, epsNode);
        }
    }
    freeStack(stack);
    stack = NULL;
}

/*
 * Prints parse tree in prefix notation
 */
void printParseTree() {
    printTree(root);
}

/*
 * Deallocates memory from each node of parse tree
 */
void freeParseTree() {
    freeTree(root);
    root = NULL;
}