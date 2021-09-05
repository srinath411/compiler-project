#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "token_stream.h"

TokenEle* tokenStream = NULL;
TokenEle* last = NULL;
TokenEle* start = NULL;

TokenEle* createNewTokenEle(Lexeme* lexeme, Token token, int lineNo, int hasError) {
    TokenEle* ele = (TokenEle*) malloc(sizeof(TokenEle));
    ele ->lexeme = lexeme;
    ele ->token = token;
    ele ->lineNo = lineNo;
    ele ->next = NULL;
    ele ->hasError = hasError;
    return ele;
}

void initTokenStream() {
    tokenStream = NULL;
    last = NULL;
    start = NULL;
}

void appendTokenEle(TokenEle* ele) {
    if (tokenStream == NULL) {
        tokenStream = ele;
        last = ele;
        start = ele;
    } else {
        last ->next = ele;
        last = ele;
    }
}

TokenEle* getTokenFromStream() {
    if (start == NULL) {
        return NULL;
    } else {
        TokenEle* temp = start;
        start = start ->next;
        return temp;
    }
}

void freeTokenEle(TokenEle* ele) {
    if (ele ->lexeme != NULL) {
        free(ele ->lexeme);
        ele ->lexeme = NULL;
    }
    ele ->next = NULL;
    free(ele);
    ele = NULL;
}

void freeTokenStream() {
    TokenEle* temp = NULL;
    while(tokenStream != NULL) {
        temp = tokenStream;
        tokenStream = tokenStream ->next;
        freeTokenEle(temp);
    }
    temp = NULL;
    last = NULL;
}

void printTokenStream() {
    TokenEle* head = tokenStream;
    while(head != NULL) {
        printf("Line %d: Token %s\n", head ->lineNo, getTokenStr(head ->token));
        head = head ->next;
    }
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
    TokenEle* ele = createNewTokenEle(lexeme, token, lineNo, 0);
    appendTokenEle(ele);
}