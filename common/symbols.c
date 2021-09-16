#include<stdlib.h>
#include "symbols.h"
#include "../utils/hash_table.h"

char* tokenStrs[] = {"ASSIGN_OP", "COMMENT", "FIELD_ID", "ID", "NUM", "R_NUM", "FUN_ID", "RECORD_ID", "WITH", "PARAMETERS", "END",
"WHILE", "INT", "REAL", "TYPE", "MAIN", "GLOBAL", "PARAMETER", "LIST", "SQL", "SQR", "INPUT", "OUTPUT", "COMMA", "SEM", "COLON",
"DOT", "END_WHILE", "OP", "CL", "IF", "THEN", "END_IF", "READ", "WRITE", "RETURN", "PLUS", "MINUS", "MUL", "DIV", "CALL", "RECORD",
"END_RECORD", "ELSE", "AND", "OR", "NOT", "LT", "GT", "LE", "GE", "EQ", "NE", "DOLLAR", "EPS", "ERR"};

char* nonTerminalStrs[] = {"program", "otherFunctions", "function", "inputParams", "outputParams", "paramList", "params", "param",
"type", "primitiveType", "userDefType", "otherParams", "stmts", "recordDefinitions", "recordDefinition", "fieldStmts", "fieldStmt",
"otherFieldStmts", "declStmts", "declStmt", "global", "otherStmts", "stmt", "assignStmt", "generalId", "fieldRef", "exp", 
"restExp", "mul", "restMul", "var", "allId", "iterStmt", "boolExp", "restBool", "andExp", "restAnd", "boolVar", "boolComp", "boolOp",
"condStmt", "optionalElse", "callStmt", "optionalReturn", "ids", "otherIds", "allIdList", "allIds", "otherAlls", "ioStmt", "returnStmt",
"mainFunction"};

HashTable* tokenTable = NULL;
HashTable* nonTerminalTable = NULL;

/*
 * Stores token strings in one hash table and non terminal strings in another hash table
 */
void storeAllSymbols() {
    tokenTable = initHashTable(tokenStrs, NUM_TOKENS);
    nonTerminalTable = initHashTable(nonTerminalStrs, NUM_NON_TERMINALS);
}

/*
 * Token String --> Token
 */
int findTokenFromStr(char* str) {
    return findEleInTable(tokenTable, str);
}

/*
 * Non Terminal String --> Non Terminal
 */
int findNonTerminalFromString(char* str) {
    return findEleInTable(nonTerminalTable, str);
}

/*
 * Token --> Token String
 */
char* getTokenStr(Token token) {
    return tokenStrs[token];
}

/*
 * Non Terminal --> Non Terminal String
 */
char* getNonTerminalStr(NonTerminal nonTerminal) {
    return nonTerminalStrs[nonTerminal];
}

/*
 * Deallocates memory from both token and non terminal hash tables
 */
void freeTokenAndNonTerminalTables() {
    freeHashTable(tokenTable);
    freeHashTable(nonTerminalTable);
    tokenTable = NULL;
    nonTerminalTable = NULL;
}

//debug
void printTokenAndNonTerminalTables() {
    printHashTable(tokenTable);
    printHashTable(nonTerminalTable);
}