#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

void printSyntaxError(int lineNo, int errorType, char* str1, char* str2);
void printParsingError(int lineNo, int errorType, char* expStr, char* gotStr);

#endif