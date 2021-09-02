#ifndef TOKEN_STREAM_H
#define TOKEN_STREAM_H
#include "../common/symbols.h"

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
Lexeme* getCopyOfLexeme(Lexeme* l1, Token token);
void addDollarAtEnd();

#endif