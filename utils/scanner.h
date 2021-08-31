#ifndef SCANNER_H
#define SCANNER_H

int initScanner(char* filename);
void closeScanner();
char nextChar();
void skipChar();
void retractChar();
char* getLexemeFromBuf();

#endif