#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "token_stream.h"
#include "../common/error_handler.h"

TokenEle* tokenStream = NULL;
TokenEle* last = NULL;
TokenEle* start = NULL;

/*
 * Initializes and returns new TokenEle
 */
TokenEle* createNewTokenEle(char* lexeme, Token token, int lineNo, int hasError) {
    TokenEle* ele = (TokenEle*) malloc(sizeof(TokenEle));
    ele ->lexeme = lexeme;
    ele ->token = token;
    ele ->lineNo = lineNo;
    ele ->next = NULL;
    ele ->hasError = hasError;
    return ele;
}

/*
 * Initializes Token Stream
 */
void initTokenStream() {
    tokenStream = NULL;
    last = NULL;
    start = NULL;
}

/*
 * Appends TokenEle to Token Stream
 */
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

/*
 * Appends $ at the end of Token Stream to simplify parsing
 */
void addDollarAtEnd() {
    char* lexeme = (char*) malloc (2 * sizeof(char));
    strcpy(lexeme, "$");
    Token token = DOLLAR;
    int lineNo = (last == NULL) ? 0 : last ->lineNo;
    TokenEle* ele = createNewTokenEle(lexeme, token, lineNo, 0);
    appendTokenEle(ele);
}

/*
 * Returns next TokenEle from Token Stream
 */
TokenEle* getTokenFromStream() {
    if (start == NULL) {
        return NULL;
    } else {
        TokenEle* temp = start;
        start = start ->next;
        if (temp ->hasError) {
            printSyntaxError(temp ->lineNo, temp ->hasError, temp ->lexeme, "");
        }
        return temp;
    }
}

/*
 * Deallocates memory from TokenEle
 */
void freeTokenEle(TokenEle* ele) {
    if (ele ->lexeme != NULL) {
        free(ele ->lexeme);
        ele ->lexeme = NULL;
    }
    ele ->next = NULL;
    free(ele);
    ele = NULL;
}

/*
 * Deallocates memory from Token Stream
 */
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

/*
 * Prints the line number, token, and lexeme for each TokenEle in Token Stream
 */
void printTokenStream() {
    TokenEle* head = tokenStream;
    while(head != NULL) {
        printf("Line %d: %s %s\n", head ->lineNo, getTokenStr(head ->token), head ->lexeme);
        head = head ->next;
    }
}