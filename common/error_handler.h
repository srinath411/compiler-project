#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

void printSyntaxError(int lineNo, int errorType, char* lexeme);
void printParsingError(int lineNo, int errorType, char* expStr, char* gotStr);

#endif