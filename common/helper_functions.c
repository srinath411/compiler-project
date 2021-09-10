#include<stdio.h>

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