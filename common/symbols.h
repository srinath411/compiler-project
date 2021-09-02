#ifndef SYMBOLS_H
#define SYMBOLS_H
#define NUM_TOKENS 55
#define NUM_NON_TERMINALS 52

enum Token {ASSIGN_OP, COMMENT, FIELD_ID, ID, NUM, R_NUM, FUN_ID, RECORD_ID, WITH, PARAMETERS, END, WHILE, INT, REAL, TYPE, MAIN, GLOBAL,
PARAMETER, LIST, SQL, SQR, INPUT, OUTPUT, COMMA, SEM, COLON, DOT, END_WHILE, OP, CL, IF, THEN, END_IF, READ, WRITE, RETURN, PLUS, MINUS,
MUL, DIV, CALL, RECORD, END_RECORD, ELSE, AND, OR, NOT, LT, GT, LE, GE, EQ, NE, DOLLAR, EPS};

enum NonTerminal {program, otherFunctions, function, inputParams, outputParams, paramList, params, param, type, primitiveType,
userDefType, otherParams, stmts, recordDefinitions, recordDefinition, fieldStmts, fieldStmt, otherFieldStmts, declStmts, declStmt,
global, otherStmts, stmt, assignStmt, generalId, fieldRef, exp, restExp, mul, restMul, var, allId, iterStmt, boolExp, restBool,
andExp, restAnd, boolVar, boolComp, boolOp, condStmt, optionalElse, callStmt, optionalReturn, ids, otherIds, allIdList, allIds, otherAlls,
ioStmt, returnStmt, mainFunction};

typedef enum Token Token;
typedef enum NonTerminal NonTerminal;

typedef union Symbol {
    Token token;
    NonTerminal nonTerminal;
} Symbol;

void storeAllSymbols();
int findTokenFromStr(char* str);
int findNonTerminalFromString(char* str);
char* getTokenStr(Token token);
char* getNonTerminalStr(NonTerminal nonTerminal);
void freeTokenAndNonTerminalTables();
void printTokenAndNonTerminalTables();

#endif