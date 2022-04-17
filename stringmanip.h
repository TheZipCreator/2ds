#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define boolean int
#define true 1
#define false 0

char* substring(char* text, int start, int end) {
    char* result = (char*)malloc(end - start + 1);
    for(int i = start; i < end; i++) {
        result[i - start] = text[i];
    }
    result[end - start] = '\0';
    return result;
}

char* toLowerCase(char* str) {
    char* result = (char*)malloc(strlen(str) + 1);
    for(int i = 0; i < strlen(str); i++) {
        result[i] = tolower(str[i]);
    }
    result[strlen(str)] = '\0';
    return result;
}

boolean isNumericString(char* str) {
    int numDots = 0;
    for(int i = 0; i < strlen(str); i++) {
        if(!isdigit(str[i]) && (str[i] != '.' || numDots > 0) && str[i] != '-') {
            return false;
        }
        if(str[i] == '.') numDots++;
    }
    return true;
}

boolean isIntegerString(char* str) {
    for(int i = 0; i < strlen(str); i++) {
        if(!isdigit(str[i]) && str[i] != '-') {
            return false;
        }
    }
    return true;
}

boolean isAlphabeticString(char* str) {
    for(int i = 0; i < strlen(str); i++) {
        if(!isalpha(str[i])) {
            return false;
        }
    }
    return true;
}