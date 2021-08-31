#ifndef GRAMMAR_H
#define GRAMMAR_H
#define NUM_NON_TERMINALS 52
#define NUM_RULES 89

typedef struct GrammarEle {
    int symbol;
    int isTerminal;
    struct GrammarEle* next;
} GrammarEle;

enum NonTerminal {program, otherFunctions, function, inputParams, outputParams, paramList, params, param, type, primitiveType,
userDefType, otherParams, stmts, recordDefinitions, recordDefinition, fieldStmts, fieldStmt, otherFieldStmts, declStmts, declStmt,
global, otherStmts, stmt, assignStmt, generalId, fieldRef, exp, restExp, mul, restMul, var, allId, iterStmt, boolExp, restBool,
andExp, restAnd, boolVar, boolComp, boolOp, condStmt, optionalElse, callStmt, optionalReturn, ids, otherIds, allIdList, allIds, otherAlls,
ioStmt, returnStmt, mainFunction};

typedef enum NonTerminal NonTerminal;

void initGrammarRules();
int getFirstOccurence(NonTerminal nonTerminal);
int getLastOccurence(NonTerminal nonTerminal);
GrammarEle* getGrammarRule(int ruleNo);
void freeTokenAndNonTerminalTables();
void freeGrammarRules();
void printTokenAndNonTerminalTables();
void printGrammarRules();
char* getNonTerminalStr(int i);
char* getTokenStr(int i);

#endif