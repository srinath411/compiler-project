#include<stdio.h>
#include<string.h>
#define MAX_CAUSE_SIZE 50
#define MAX_DESC_SIZE 100

void printError(char* errorClass, int lineNo, char* errorCause, char* errorDesc) {
    printf("%s Line %d: %s %s\n", errorClass, lineNo, errorCause, errorDesc);
}

void printLexicalError(int lineNo, int errorType, char* lexeme) {
    char errorCause[MAX_CAUSE_SIZE] = {'\0'};
    char errorDesc[MAX_DESC_SIZE] = {'\0'};
    switch(errorType) {
        case 1:
            strcpy(errorCause, "Invalid Sequence ");
            strcat(errorCause, lexeme);
            strcpy(errorDesc, "");
            break;
        case 2:
            strcpy(errorCause, "Invalid Real Number ");
            strcat(errorCause, lexeme);
            strcpy(errorDesc, "Real Numbers must have exactly two digits after decimal");
            break;
        case 3:
            strcpy(errorCause, "Invalid Character ");
            strcat(errorCause, lexeme);
            strcpy(errorDesc, "");
            break;
    }
    printError("SYNTAX ERROR", lineNo, errorCause, errorDesc);
}

void printParsingError(int lineNo, int errorType, char* expStr, char* gotStr) {
    char errorCause[MAX_CAUSE_SIZE] = {'\0'};
    char errorDesc[MAX_DESC_SIZE] = {'\0'};
    switch(errorType) {
        case 4:
            strcpy(errorCause, "Invalid Occurence of ");
            strcat(errorCause, gotStr);
            strcpy(errorDesc, "");
            break;
        case 5:
            strcpy(errorCause, "Expected ");
            strcat(errorCause, expStr);
            strcat(errorCause, " but not found");
            strcpy(errorDesc, "");
            break;
    }
    printError("SYNTAX ERROR", lineNo, errorCause, errorDesc);
}