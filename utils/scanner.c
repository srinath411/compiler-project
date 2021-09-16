#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define READ_BUF_SIZE 1024

char readBuf[READ_BUF_SIZE];
char* lexemeBuf;
int startIndex, currentIndex;
FILE *fp;

/*
 * Initializes Scanner
 * Returns 0 on success and 1 on failure
 */
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

/*
 * Closes Input file and frees allocated memory
 */
void closeScanner() {
    if (fp != NULL) {
        fclose(fp);
        fp = NULL;
    }
    free(lexemeBuf);
    lexemeBuf = NULL;
}

/*
 * Appends readBuf to lexemeBuf
 */
void concatBufAndLexStrings() {
    if (startIndex < currentIndex) {
        int lexemeLen = currentIndex - startIndex;
        if (lexemeBuf == NULL) {
            // Create lexemeBuf
            lexemeBuf = (char*) malloc((lexemeLen + 1) * sizeof(char));
            for(int i = startIndex; i < currentIndex; i++) {
                lexemeBuf[i - startIndex] = readBuf[i];
            }
            lexemeBuf[lexemeLen] = '\0';
        } else {
            // Reallocate lexemeBuf and then append readBuf
            int lexBufLen = strlen(lexemeBuf);
            lexemeBuf = (char*) realloc (lexemeBuf, (lexBufLen + lexemeLen + 1) * sizeof(char));
            for(int i = startIndex; i < currentIndex; i++) {
                lexemeBuf[i - startIndex + lexBufLen] = readBuf[i];
            }
            lexemeBuf[lexemeLen + lexBufLen] = '\0';
        }
    }
}

/*
 * Returns string scanned after last reset
 */
char* getLexemeFromBuf() {
    concatBufAndLexStrings(); // Some part of string may be present in lexemeBuf, so always concatenate
    startIndex = currentIndex; // Should consider chars from current index
    char* lexeme = lexemeBuf;
    lexemeBuf = NULL; // Reset lexemeBuf
    return lexeme;
}

/*
 * Scans and returns the next character in the input file
 * Returns EOF if no character left
 */
char nextChar() {
    while(readBuf[currentIndex] == '\0') {
        // Reached end of readBuf
        // Append readBuf to lexemeBuf and reset readBuf
        concatBufAndLexStrings();
        startIndex = currentIndex = 0;
        readBuf[0] = '\0';

        // If next line is present, read from file
        // Else store EOF
        if (!feof(fp)) {
            fgets(readBuf, READ_BUF_SIZE, fp);
        } else {
            readBuf[0] = EOF;
        }
    }
    return readBuf[currentIndex++];
}

/*
 * Skips all scanned chars
 */
void skipChar() {
    startIndex = currentIndex;
}

/*
 * Retract the last scanned char
 */
void retractChar() {
    if (currentIndex > startIndex) {
        currentIndex--;
    }
}