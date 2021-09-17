#include<stdio.h>
#include<stdlib.h>
#include "parse_tree.h"

/*
 * Creates and returns new TreeNode with given info
 */
TreeNode* createTreeNode(int symbol, NodeType nodeType) {
    TreeNode* node = (TreeNode*) malloc (sizeof(TreeNode));
    node ->lchild = node ->rsib = node ->parent = NULL;
    node ->nodeType = nodeType;
    NodeInfo* nodeInfo = (NodeInfo*) malloc (sizeof(NodeInfo));
    node ->info = nodeInfo;
    if (nodeType == parentNodeType) {
        ParentNodeInfo* info = (ParentNodeInfo*) malloc (sizeof(ParentNodeInfo));
        info ->nonTerminal = symbol;
        nodeInfo ->parent = info;
    } else {
        LeafNodeInfo* info = (LeafNodeInfo*) malloc (sizeof(LeafNodeInfo));
        info ->token = symbol;
        info ->lexeme = NULL;
        nodeInfo ->leaf = info;
    }
    return node;
}

/*
 * Sets required pointers to make childNode a child of parentNode
 */
void addChildToNode(TreeNode* parentNode, TreeNode* childNode) {
    childNode ->parent = parentNode;
    childNode ->rsib = parentNode ->lchild;
    parentNode ->lchild = childNode;
}

/*
 * Prints parse tree in prefix format
 */
void printTree(TreeNode* node) {
    if (node == NULL) {
        return;
    }
    printf("Line %d: ", node ->lineNo);
    if (node ->nodeType == parentNodeType) {
        printf("%s %d\n", getNonTerminalStr(node ->info ->parent ->nonTerminal), node ->info ->parent ->ruleNo);
    } else {
        Token token = node ->info ->leaf ->token;
        printf("%s ", getTokenStr(token));
        if (token == EPS) {
            printf("epsilon\n");
        } else if (node ->info ->leaf ->lexeme != NULL) {
            printf("%s\n", node ->info ->leaf ->lexeme);
        } else {
            printf("null\n");
        }
    }
    printTree(node ->lchild);
    printTree(node ->rsib);
}

/*
 * Frees parse tree
 */
void freeTree(TreeNode* node) {
    if (node == NULL) {
        return;
    }
    freeTree(node ->lchild);
    freeTree(node ->rsib);
    node ->lchild = node ->rsib = node ->parent = NULL;
    if (node ->nodeType == parentNodeType) {
        free(node ->info ->parent);
        node ->info ->parent = NULL;
    } else {
        free(node ->info ->leaf);
        node ->info ->leaf = NULL;
    }
    free(node ->info);
    node ->info = NULL;
    free(node);
    node = NULL;
}