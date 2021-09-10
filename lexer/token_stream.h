#ifndef TOKEN_STREAM_H
#define TOKEN_STREAM_H
#include "../common/symbols.h"

typedef struct TokenEle {
    Token token;
    char* lexeme;
    int lineNo;
    int hasError;
    struct TokenEle* next;
} TokenEle;

TokenEle* createNewTokenEle(char* lexeme, Token token, int lineNo, int hasError);
void initTokenStream();
void appendTokenEle(TokenEle* ele);
TokenEle* getTokenFromStream();
void freeTokenEle(TokenEle* ele);
void freeTokenStream();
void printTokenStream();
void addDollarAtEnd();

#endif