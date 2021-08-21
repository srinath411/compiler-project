#ifndef TOKENS_H
#define TOKENS_H

enum Token {ASSIGN_OP, COMMENT, FIELD_ID, ID, NUM, R_NUM, RECORD_ID, WITH, PARAMETERS, END, WHILE, INT, REAL, TYPE, MAIN, GLOBAL,
PARAMETER, LIST, SQL, SQR, INPUT, OUTPUT, COMMA, SEM, COLON, DOT, END_WHILE, OP, CL, IF, THEN, END_IF, READ, WRITE, RETURN, PLUS, MINUS,
MUL, DIV, CALL, RECORD, END_RECORD, ELSE, AND, OR, NOT, LT, GT, LE, GE, EQ, NE};

typedef enum Token Token;

typedef struct tokenEle {
    Token token;
    char* lexeme;
    int lineNo;
    struct tokenEle* next;
}tokenEle;

#endif