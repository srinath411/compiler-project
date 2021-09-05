#ifndef TOKEN_STREAM_H
#define TOKEN_STREAM_H
#include "../common/symbols.h"

typedef union Lexeme
{
    char* str;
    int num;
    float rnum;
} Lexeme;


typedef struct TokenEle {
    Token token;
    Lexeme* lexeme;
    int lineNo;
    int hasError;
    struct TokenEle* next;
} TokenEle;

TokenEle* createNewTokenEle(Lexeme* lexeme, Token token, int lineNo, int hasError);
void initTokenStream();
void appendTokenEle(TokenEle* ele);
TokenEle* getTokenFromStream();
void freeTokenEle(TokenEle* ele);
void freeTokenStream();
void printTokenStream();
Lexeme* getEmptyLexeme();
Lexeme* getCopyOfLexeme(Lexeme* l1, Token token);
void addDollarAtEnd();

#endif