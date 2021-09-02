#include<stdlib.h>
#include "symbols.h"
#include "../utils/hash_table.h"
#define TOKEN_BUCKETS 8
#define NON_TERMINAL_BUCKETS 8

char* tokenStrs[] = {"ASSIGN_OP", "COMMENT", "FIELD_ID", "ID", "NUM", "R_NUM", "FUN_ID", "RECORD_ID", "WITH", "PARAMETERS", "END",
"WHILE", "INT", "REAL", "TYPE", "MAIN", "GLOBAL", "PARAMETER", "LIST", "SQL", "SQR", "INPUT", "OUTPUT", "COMMA", "SEM", "COLON",
"DOT", "END_WHILE", "OP", "CL", "IF", "THEN", "END_IF", "READ", "WRITE", "RETURN", "PLUS", "MINUS", "MUL", "DIV", "CALL", "RECORD",
"END_RECORD", "ELSE", "AND", "OR", "NOT", "LT", "GT", "LE", "GE", "EQ", "NE", "DOLLAR", "EPS"};

char* nonTerminalStrs[] = {"program", "otherFunctions", "function", "inputParams", "outputParams", "paramList", "params", "param",
"type", "primitiveType", "userDefType", "otherParams", "stmts", "recordDefinitions", "recordDefinition", "fieldStmts", "fieldStmt",
"otherFieldStmts", "declStmts", "declStmt", "global", "otherStmts", "stmt", "assignStmt", "generalId", "fieldRef", "exp", 
"restExp", "mul", "restMul", "var", "allId", "iterStmt", "boolExp", "restBool", "andExp", "restAnd", "boolVar", "boolComp", "boolOp",
"condStmt", "optionalElse", "callStmt", "optionalReturn", "ids", "otherIds", "allIdList", "allIds", "otherAlls", "ioStmt", "returnStmt",
"mainFunction"};

hashEle** tokenTable = NULL;
hashEle** nonTerminalTable = NULL;

void storeAllSymbols() {
    tokenTable = initHashTable(tokenStrs, NUM_TOKENS, TOKEN_BUCKETS);
    nonTerminalTable = initHashTable(nonTerminalStrs, NUM_NON_TERMINALS, NON_TERMINAL_BUCKETS);
}

int findTokenFromStr(char* str) {
    return findEleInTable(tokenTable, TOKEN_BUCKETS, str);
}

int findNonTerminalFromString(char* str) {
    return findEleInTable(nonTerminalTable, NON_TERMINAL_BUCKETS, str);
}

char* getTokenStr(Token token) {
    return tokenStrs[token];
}

char* getNonTerminalStr(NonTerminal nonTerminal) {
    return nonTerminalStrs[nonTerminal];
}

void freeTokenAndNonTerminalTables() {
    freeHashTable(tokenTable, TOKEN_BUCKETS);
    freeHashTable(nonTerminalTable, NON_TERMINAL_BUCKETS);
    tokenTable = NULL;
    nonTerminalTable = NULL;
}

//debug
void printTokenAndNonTerminalTables() {
    printHashTable(tokenTable, TOKEN_BUCKETS);
    printHashTable(nonTerminalTable, NON_TERMINAL_BUCKETS);
}