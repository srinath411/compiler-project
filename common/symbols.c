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
    tokenTable = initHashTable(NUM_TOKENS);
    for(int i = 0; i < NUM_TOKENS; i++) {
        Token* val = (Token*) malloc(sizeof(Token));
        *val = i;
        hashEleInTable(tokenTable, tokenStrs[i], val);
    }
    nonTerminalTable = initHashTable(NUM_NON_TERMINALS);
    for(int i = 0; i < NUM_NON_TERMINALS; i++) {
        NonTerminal* val = (NonTerminal*) malloc(sizeof(NonTerminal));
        *val = i;
        hashEleInTable(nonTerminalTable, nonTerminalStrs[i], val);
    }
}

/*
 * Token String --> Token
 */
int findTokenFromStr(char* str) {
    Token* tokenPtr = (Token*)findEleInTable(tokenTable, str);
    return (tokenPtr == NULL) ? -1 : *tokenPtr;
}

/*
 * Non Terminal String --> Non Terminal
 */
int findNonTerminalFromString(char* str) {
    NonTerminal* nonTerminalPtr = (NonTerminal*) findEleInTable(nonTerminalTable, str);
    return (nonTerminalPtr == NULL) ? -1 : *nonTerminalPtr;
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