#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "keywords.h"
#include "../lexer/token_stream.h"
#include "../utils/scanner.h"

int isInRange(char c, char low, char high) {
    return (c >= low) && (c <= high);
}

int stringToInteger(char* str) {
    int num;
    sscanf(str, "%d", &num);
    return num;
}

float stringToFloat(char* str) {
    float f;
    sscanf(str, "%f", &f);
    return f;
}

void addStrLexToStream(char* lex, Token token, int lineNo, int hasError) {
    Lexeme* lexeme = getEmptyLexeme();
    lexeme ->str = lex;
    TokenEle* ele = createNewTokenEle(lexeme, token, lineNo, hasError);
    appendTokenEle(ele);
}

void addNumToStream(char* lex, Token token, int lineNo, int hasError) {
    int num = stringToInteger(lex);
    Lexeme* lexeme = getEmptyLexeme();
    lexeme ->num = num;
    TokenEle* ele = createNewTokenEle(lexeme, token, lineNo, hasError);
    appendTokenEle(ele);
}

void addRealNumToStream(char* lex, Token token, int lineNo, int hasError) {
    float rnum = stringToFloat(lex);
    Lexeme* lexeme = getEmptyLexeme();
    lexeme ->rnum = rnum;
    TokenEle* ele = createNewTokenEle(lexeme, token, lineNo, hasError);
    appendTokenEle(ele);
}

void findAndAddTokens() {
    int dfaState = 0, lineNo = 1, stateSet = 0;
    char c;
    char *lexeme = NULL, *expectedSeq = NULL;
    Token reqdToken = -1;
    while(1) {
        switch(dfaState) {
            case 0:
                stateSet = 0;
                c = nextChar();
                switch(c) {
                    case EOF:
                        return;
                    case '\0':
                    case ' ':
                    case '\t':
                    case '\f':
                    case '\v':
                    case '\r':
                        skipChar();
                        stateSet = 1;
                        break;
                    case '\n':
                        lineNo++;
                        skipChar();
                        stateSet = 1;
                        break;
                    case '[':
                        reqdToken = SQL;
                        dfaState = 1;
                        break;
                    case ']':
                        reqdToken = SQR;
                        dfaState = 1;
                        break;
                    case '(':
                        reqdToken = OP;
                        dfaState = 1;
                        break;
                    case ')':
                        reqdToken = CL;
                        dfaState = 1;
                        break;
                    case '+':
                        reqdToken = PLUS;
                        dfaState = 1;
                        break;
                    case '-':
                        reqdToken = MINUS;
                        dfaState = 1;
                        break;
                    case '*':
                        reqdToken = MUL;
                        dfaState = 1;
                        break;
                    case '/':
                        reqdToken = DIV;
                        dfaState = 1;
                        break;
                    case ',':
                        reqdToken = COMMA;
                        dfaState = 1;
                        break;
                    case ';':
                        reqdToken = SEM;
                        dfaState = 1;
                        break;
                    case ':':
                        reqdToken = COLON;
                        dfaState = 1;
                        break;
                    case '.':
                        reqdToken = DOT;
                        dfaState = 1;
                        break;
                    case '~':
                        reqdToken = NOT;
                        dfaState = 1;
                        break;
                    case '&':
                        dfaState = 4;
                        break;
                    case '<':
                        dfaState = 6;
                        break;
                    case '>':
                        dfaState = 9;
                        break;
                    case '!':
                        dfaState = 10;
                        break;
                    case '=':
                        dfaState = 11;
                        break;
                    case '@':
                        dfaState = 12;
                        break;
                    case '_':
                        dfaState = 21;
                        break;
                    case '#':
                        dfaState = 24;
                        break;
                    case '%':
                        dfaState = 32;
                        break;
                }
                if (stateSet || dfaState > 0) {
                    break;
                } else if (isInRange(c, '0', '9')) {
                    dfaState = 14;
                } else if (isInRange(c, 'b', 'd')) {
                    dfaState = 17;
                } else if (isInRange(c, 'a', 'z')) {
                    dfaState = 20;
                } else {
                    dfaState = 29;
                }
                break;
            case 1:
                lexeme = getLexemeFromBuf();
                addStrLexToStream(lexeme, reqdToken, lineNo, 0);
                dfaState = 0;
                break;
            case 2:
                retractChar();
                lexeme = getLexemeFromBuf();
                addStrLexToStream(lexeme, reqdToken, lineNo, 0);
                dfaState = 0;
                break;
            case 3:
                retractChar();
                lexeme = getLexemeFromBuf();
                Token tempToken = getKeywordIfPresent(lexeme);
                if (tempToken != -1) {
                    reqdToken = tempToken;
                }
                addStrLexToStream(lexeme, reqdToken, lineNo, 0);
                dfaState = 0;
                break;
            case 4:
                c = nextChar();
                if (c == '&') {
                    dfaState = 5;
                } else {
                    expectedSeq = "&&&";
                    reqdToken = AND;
                    dfaState = 26;
                }
                break;
            case 5:
                c = nextChar();
                if (c == '&') {
                    reqdToken = AND;
                    dfaState = 1;
                } else {
                    expectedSeq = "&&&";
                    reqdToken = AND;
                    dfaState = 26;
                }
                break;
            case 6:
                c = nextChar();
                if (c == '-') {
                    dfaState = 7;
                } else if (c == '=') {
                    reqdToken = LE;
                    dfaState = 1;
                } else {
                    reqdToken = LT;
                    dfaState = 2;
                }
                break;
            case 7:
                c = nextChar();
                if (c == '-') {
                    dfaState = 8;
                } else {
                    expectedSeq = "<---";
                    reqdToken = ASSIGN_OP;
                    dfaState = 26;
                }
                break;
            case 8:
                c = nextChar();
                if (c == '-') {
                    reqdToken = ASSIGN_OP;
                    dfaState = 1;
                } else {
                    expectedSeq = "<---";
                    reqdToken = ASSIGN_OP;
                    dfaState = 26;
                }
                break;
            case 9:
                c = nextChar();
                if (c == '=') {
                    reqdToken = GE;
                    dfaState = 1;
                } else {
                    reqdToken = GT;
                    dfaState = 2;
                }
                break;
            case 10:
                c = nextChar();
                if (c == '=') {
                    reqdToken = NE;
                    dfaState = 1;
                } else {
                    expectedSeq = "!=";
                    reqdToken = NE;
                    dfaState = 26;
                }
                break;
            case 11:
                c = nextChar();
                if (c == '=') {
                    reqdToken = EQ;
                    dfaState = 1;
                } else {
                    expectedSeq = "==";
                    reqdToken = EQ;
                    dfaState = 26;
                }
                break;
            case 12:
                c = nextChar();
                if (c == '@') {
                    dfaState = 13;
                } else {
                    expectedSeq = "@@@";
                    reqdToken = OR;
                    dfaState = 26;
                }
                break;
            case 13:
                c = nextChar();
                if (c == '@') {
                    reqdToken = OR;
                    dfaState = 1;
                } else {
                    expectedSeq = "@@@";
                    reqdToken = OR;
                    dfaState = 26;
                }
                break;
            case 14:
                c = nextChar();
                if (isInRange(c, '0', '9')) {
                    dfaState = 14;
                } else if (c == '.') {
                    dfaState = 15;
                } else {
                    dfaState = 30;
                }
                break;
            case 15:
                c = nextChar();
                if (isInRange(c, '0', '9')) {
                    dfaState = 16;
                } else {
                    dfaState = 27;
                }
                break;
            case 16:
                c = nextChar();
                if (isInRange(c, '0', '9')) {
                    dfaState = 31;
                } else {
                    dfaState = 27;
                }
                break;
            case 17:
                c = nextChar();
                if (isInRange(c, '2', '7')) {
                    dfaState = 18;
                } else if (isInRange(c, 'a', 'z')) {
                    dfaState = 20;
                } else {
                    reqdToken = FIELD_ID;
                    dfaState = 3;
                }
                break;
            case 18:
                c = nextChar();
                if (isInRange(c, 'b', 'd')) {
                    dfaState = 18;
                } else if (isInRange(c, '2', '7')){
                    dfaState = 19;
                } else {
                    reqdToken = ID;
                    dfaState = 2;
                }
                break;
            case 19:
                c = nextChar();
                if (isInRange(c, '2', '7')) {
                    dfaState = 19;
                } else {
                    reqdToken = ID;
                    dfaState = 2;
                }
                break;
            case 20:
                c = nextChar();
                if (isInRange(c, 'a', 'z')) {
                    dfaState = 20;
                } else {
                    reqdToken = FIELD_ID;
                    dfaState = 3;
                }
                break;
            case 21:
                c = nextChar();
                if (isInRange(c, 'a', 'z') || isInRange(c, 'A', 'Z')) {
                    dfaState = 22;
                } else {
                    reqdToken = FUN_ID;
                    dfaState = 28;
                }
                break;
            case 22:
                c = nextChar();
                if (isInRange(c, 'a', 'z') || isInRange(c, 'A', 'Z')) {
                    dfaState = 22;
                } else if (isInRange(c, '0', '9')) {
                    dfaState = 23;
                } else {
                    reqdToken = FUN_ID;
                    dfaState = 3;
                }
                break;
            case 23:
                c = nextChar();
                if (isInRange(c, '0', '9')) {
                    dfaState = 23;
                } else {
                    reqdToken = FUN_ID;
                    dfaState = 3;
                }
                break;
            case 24:
                c = nextChar();
                if (isInRange(c, 'a', 'z')) {
                    dfaState = 25;
                } else {
                    reqdToken = RECORD_ID;
                    dfaState = 28;
                }
                break;
            case 25:
                c = nextChar();
                if (isInRange(c, 'a', 'z')) {
                    dfaState = 25;
                } else {
                    reqdToken = RECORD_ID;
                    dfaState = 2;
                }
                break;
            case 26:
                retractChar();
                lexeme = getLexemeFromBuf();
                addStrLexToStream(lexeme, reqdToken, lineNo, 1);
                dfaState = 0;
                break;
            case 27:
                retractChar();
                lexeme = getLexemeFromBuf();
                addStrLexToStream(lexeme, R_NUM, lineNo, 2);
                dfaState = 0;
                break;
            case 28:
                retractChar();
                lexeme = getLexemeFromBuf();
                addStrLexToStream(lexeme, reqdToken, lineNo, 1);
                dfaState = 0;
                break;
            case 29:
                lexeme = getLexemeFromBuf();
                addStrLexToStream(lexeme, ERR, lineNo, 3);
                dfaState = 0;
                break;
            case 30:
                retractChar();
                lexeme = getLexemeFromBuf();
                addNumToStream(lexeme, NUM, lineNo, 0);
                dfaState = 0;
                break;
            case 31:
                lexeme = getLexemeFromBuf();
                addRealNumToStream(lexeme, R_NUM, lineNo, 0);
                dfaState = 0;
                break;
            case 32:
                c = nextChar();
                skipChar();
                if (c == '\n') {
                    lineNo++;
                    dfaState = 0;
                    break;
                }
        }
    }
}

void tokenizeCode(char* filename) {
    int hasError = initScanner(filename);
    if (hasError) {
        printf("Error opening file\n");
        return;
    }
    initKeywordTable();
    findAndAddTokens();
    addDollarAtEnd();
    freeKeywordTable();
    closeScanner();
}