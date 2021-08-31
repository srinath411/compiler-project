#include<stdio.h>
#include<stdlib.h>
#include "parse_tree.h"

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

void addChildToNode(TreeNode* parentNode, TreeNode* childNode) {
    childNode ->parent = parentNode;
    childNode ->rsib = parentNode ->lchild;
    parentNode ->lchild = childNode;
}

void printParseTree(TreeNode* node) {
    if (node == NULL) {
        return;
    }
    printf("Line %d: ", node ->lineNo);
    if (node ->nodeType == parentNodeType) {
        printf("%s\n", getNonTerminalStr(node ->info ->parent ->nonTerminal));
    } else {
        Token token = node ->info ->leaf ->token;
        printf("%s ", getTokenStr(token));
        if (token == NUM) {
            printf("%d\n", node ->info ->leaf ->lexeme ->num);
        } else if (token == R_NUM) {
            printf("%f\n", node ->info ->leaf ->lexeme ->rnum);
        } else if(token == EPS) {
            printf("epsilon\n");
        } else {
            printf("%s\n", node ->info ->leaf ->lexeme ->str);
        }
    }
    printParseTree(node ->lchild);
    printParseTree(node ->rsib);
}

void freeParseTree(TreeNode* node) {
    if (node == NULL) {
        return;
    }
    freeParseTree(node ->lchild);
    freeParseTree(node ->rsib);
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