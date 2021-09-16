#include<stdio.h>

/*
 * Determines whether given character is within the bounds specified
 */
int isInRange(char c, char low, char high) {
    return (c >= low) && (c <= high);
}

/*
 * String --> Integer
 */
int stringToInteger(char* str) {
    int num;
    sscanf(str, "%d", &num);
    return num;
}

/*
 * String --> Float
 */
float stringToFloat(char* str) {
    float f;
    sscanf(str, "%f", &f);
    return f;
}