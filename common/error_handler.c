#include<stdio.h>
#include<string.h>
#define MAX_CAUSE_SIZE 50
#define MAX_DESC_SIZE 100

/*
 * Standard template to print errors
 */
void printError(char* errorClass, int lineNo, char* errorCause, char* errorDesc) {
    printf("%s Line %-3d %s %s\n", errorClass, lineNo, errorCause, errorDesc);
}

/*
 * Prints lexical errors based on errorType
 */
void printSyntaxError(int lineNo, int errorType, char* str1, char* str2) {
    char errorCause[MAX_CAUSE_SIZE] = {'\0'};
    char errorDesc[MAX_DESC_SIZE] = {'\0'};
    switch(errorType) {
        case 1:
            strcpy(errorCause, "Invalid Sequence ");
            strcat(errorCause, str1);
            strcpy(errorDesc, "");
            break;
        case 2:
            strcpy(errorCause, "Invalid Real Number ");
            strcat(errorCause, str1);
            strcpy(errorDesc, "Real Numbers must have exactly two digits after decimal");
            break;
        case 3:
            strcpy(errorCause, "Invalid Character ");
            strcat(errorCause, str1);
            strcpy(errorDesc, "");
            break;
        case 4:
            strcpy(errorCause, "Invalid Occurence of ");
            strcat(errorCause, str2);
            strcpy(errorDesc, "");
            break;
        case 5:
            strcpy(errorCause, "Expected ");
            strcat(errorCause, str1);
            strcat(errorCause, " but not found");
            strcpy(errorDesc, "");
            break;
    }
    printError("SYNTAX ERROR", lineNo, errorCause, errorDesc);
}