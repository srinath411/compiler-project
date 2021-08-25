#ifndef TOKENS_H
#define TOKENS_H

enum Token {ASSIGN_OP, COMMENT, FIELD_ID, ID, NUM, R_NUM, FUN_ID, RECORD_ID, WITH, PARAMETERS, END, WHILE, INT, REAL, TYPE, MAIN, GLOBAL,
PARAMETER, LIST, SQL, SQR, INPUT, OUTPUT, COMMA, SEM, COLON, DOT, END_WHILE, OP, CL, IF, THEN, END_IF, READ, WRITE, RETURN, PLUS, MINUS,
MUL, DIV, CALL, RECORD, END_RECORD, ELSE, AND, OR, NOT, LT, GT, LE, GE, EQ, NE};

typedef enum Token Token;

typedef union lexeme
{
    char* str;
    int num;
    float rnum;
} Lexeme;


typedef struct tokenEle {
    Token token;
    Lexeme* lexeme;
    int lineNo;
    struct tokenEle* next;
}tokenEle;

tokenEle* createNewTokenEle(Lexeme* lexeme, Token token, int lineNo);
void initTokenStream();
void appendTokenEle(tokenEle* ele);
tokenEle* getTokenFromStream();
void freeTokenEle(tokenEle* ele);
void freeTokenStream();
Lexeme* getEmptyLexeme();

#endif