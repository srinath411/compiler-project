#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include "../lexer/token_stream.h"
#include "grammar.h"

enum NodeType {parentNodeType, leafNodeType};
typedef enum NodeType NodeType;

typedef struct ParentNodeInfo {
    NonTerminal nonTerminal;
    int ruleNo;
} ParentNodeInfo;

typedef struct LeafNodeInfo {
    Token token;
    char* lexeme;
} LeafNodeInfo;

typedef union NodeInfo {
    ParentNodeInfo* parent;
    LeafNodeInfo* leaf;
} NodeInfo;

typedef struct TreeNode {
    NodeInfo* info;
    NodeType nodeType;
    int lineNo;
    struct TreeNode* parent;
    struct TreeNode* lchild;
    struct TreeNode* rsib;
} TreeNode;

TreeNode* createTreeNode(int symbol, NodeType nodeType);
void addChildToNode(TreeNode* parentNode, TreeNode* childNode);
void printTree(TreeNode* node);
void freeTree(TreeNode* node);

#endif