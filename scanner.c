#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define READ_BUF_SIZE 1024

char readBuf[READ_BUF_SIZE];
int startIndex, currentIndex, exceeded;
FILE *fp;

int initScanner(char* filename) {
    fp = fopen(filename, "r");
    if (fp == NULL) {
        return 1;
    }
    readBuf[0] = '\0';
    startIndex = currentIndex = exceeded = 0;
    return 0;
}

void closeScanner() {
    if (fp != NULL) {
        fclose(fp);
        fp = NULL;
    }
}

char nextChar() {
    if (readBuf[currentIndex] == '\0') {
        if (exceeded == 1) {
            if (!feof(fp)) {
                fgets(readBuf, READ_BUF_SIZE, fp);
                startIndex = currentIndex = exceeded = 0;
                return readBuf[currentIndex++];
            } else {
                return EOF;
            }
        } else {
            exceeded = 1;
            return '\0';
        }
    } else {
        return readBuf[currentIndex++];
    }
}

void skipChar() {
    startIndex = currentIndex;
}

void retractChar() {
    if (exceeded == 1) {
        exceeded = 0;
    } else if (currentIndex >= startIndex) {
        currentIndex--;
    }
}

char* getLexemeFromBuf() {
    if (startIndex > currentIndex) {
        return "";
    }
    int lexemeLen = currentIndex - startIndex;
    char* lexeme = (char*) malloc ((lexemeLen + 1) * sizeof(char));
    for(int i = startIndex; i < currentIndex; i++) {
        lexeme[i - startIndex] = readBuf[i];
    }
    lexeme[lexemeLen] = '\0';
    startIndex = currentIndex;
    return lexeme;
}