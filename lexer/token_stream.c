#include<stdlib.h>
#include<string.h>
#include "token_stream.h"

tokenEle* tokenStream = NULL;
tokenEle* last = NULL;

tokenEle* createNewTokenEle(Lexeme* lexeme, Token token, int lineNo) {
    tokenEle* ele = (tokenEle*) malloc(sizeof(tokenEle));
    ele ->lexeme = lexeme;
    ele ->token = token;
    ele ->lineNo = lineNo;
    ele ->next = NULL;
    return ele;
}

void initTokenStream() {
    tokenStream = NULL;
    last = NULL;
}

void appendTokenEle(tokenEle* ele) {
    if (tokenStream == NULL) {
        tokenStream = ele;
        last = ele;
    } else {
        last ->next = ele;
        last = ele;
    }
}

tokenEle* getTokenFromStream() {
    if (tokenStream == NULL) {
        return tokenStream;
    } else {
        tokenEle* temp = tokenStream;
        tokenStream = tokenStream ->next;
        temp ->next = NULL;
        return temp;
    }
}

void freeTokenEle(tokenEle* ele) {
    if (ele ->lexeme != NULL) {
        free(ele ->lexeme);
        ele ->lexeme = NULL;
    }
    ele ->next = NULL;
    free(ele);
    ele = NULL;
}

void freeTokenStream() {
    tokenEle* temp = NULL;
    while(tokenStream != NULL) {
        temp = tokenStream;
        tokenStream = tokenStream ->next;
        freeTokenEle(temp);
    }
    temp = NULL;
    last = NULL;
}

Lexeme* getEmptyLexeme() {
    Lexeme* lexeme = (Lexeme*) malloc (sizeof(Lexeme));
    return lexeme;
}

Lexeme* getCopyOfLexeme(Lexeme* l1, Token token) {
    Lexeme* l2 = getEmptyLexeme();
    if (token == NUM) {
        l2 ->num = l1 ->num;
    } else if (token == R_NUM) {
        l2 ->rnum = l1 ->rnum;
    } else {
        int len = strlen(l1 ->str) + 1;
        char* lex = (char*) malloc(len * (sizeof(char)));
        for (int i = 0; i < len; i++) {
            lex[i] = l1 ->str[i];
        }
        lex[len - 1] = '\0';
        l2 ->str = lex;
    }
    return l2;
}

void addDollarAtEnd() {
    Lexeme* lexeme = getEmptyLexeme();
    lexeme ->str = "$";
    Token token = DOLLAR;
    int lineNo = (last == NULL) ? 0 : last ->lineNo;
    tokenEle* ele = createNewTokenEle(lexeme, token, lineNo);
    appendTokenEle(ele);
}