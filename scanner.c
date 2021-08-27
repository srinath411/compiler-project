#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define READ_BUF_SIZE 1024

char readBuf[READ_BUF_SIZE];
char* lexemeBuf;
int startIndex, currentIndex;
FILE *fp;

int initScanner(char* filename) {
    fp = fopen(filename, "r");
    if (fp == NULL) {
        return 1;
    }
    readBuf[0] = '\0';
    startIndex = currentIndex = 0;
    lexemeBuf = NULL;
    return 0;
}

void closeScanner() {
    if (fp != NULL) {
        fclose(fp);
        fp = NULL;
    }
    lexemeBuf = NULL;
}

void concatBufAndLexStrings() {
    if (startIndex < currentIndex) {
        int lexemeLen = currentIndex - startIndex;
        if (lexemeBuf == NULL) {
            lexemeBuf = (char*) malloc((lexemeLen + 1) * sizeof(char));
            for(int i = startIndex; i < currentIndex; i++) {
                lexemeBuf[i - startIndex] = readBuf[i];
            }
            lexemeBuf[lexemeLen] = '\0';
        } else {
            int lexBufLen = strlen(lexemeBuf);
            lexemeBuf = (char*) realloc (lexemeBuf, (lexBufLen + lexemeLen + 1) * sizeof(char));
            for(int i = startIndex; i < currentIndex; i++) {
                lexemeBuf[i - startIndex + lexBufLen] = readBuf[i];
            }
            lexemeBuf[lexemeLen + lexBufLen] = '\0';
        }
    }
}

char* getLexemeFromBuf() {
    concatBufAndLexStrings();
    startIndex = currentIndex;
    char* lexeme = lexemeBuf;
    lexemeBuf = NULL;
    return lexeme;
}

char nextChar() {
    if (readBuf[currentIndex] == '\0') {
        concatBufAndLexStrings();
        startIndex = currentIndex = 0;
        if (!feof(fp)) {
            fgets(readBuf, READ_BUF_SIZE, fp);
            return readBuf[currentIndex++];
        } else {
            readBuf[0] = EOF;
            return readBuf[currentIndex++];
        }
    } else {
        return readBuf[currentIndex++];
    }
}

void skipChar() {
    startIndex = currentIndex;
}

void retractChar() {
    if (currentIndex > startIndex) {
        currentIndex--;
    }
}