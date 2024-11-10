#include "macro.h"

int cmpstr(char* str1, char* str2) {
    for (int i = 0; str1[i] != 0 || str2[i] != 0; i++) {
        if (str1[i] != str2[i])
            return 0;
    }

    return 1;
}

int isalpha(char c) {
    if ((c > 64 && c < 91) || (c > 96 && c < 123))
        return 1;
    else
        return 0;
}

int isdigit(char c) {
    if (c > 47 && c < 58)
        return 1;
    else
        return 0;
}

int ispunct(char c) {
    if ((c > 32 && c < 48) || (c > 57 && c < 65) || (c > 90 && c < 97) || (c > 122 && c < 127))
        return 1;
    else
        return 0;
}

int isparen(char c) {
    char list[7] = "(){}[]";

    for (int i = 0; list[i] != 0; i++) {
        if (c == list[i])
            return 1;
    }

    return 0;
}

int isindent(char* str, int index) {
    for (int i = 0; i < INDENT_SIZE; i++) {
        if (str[index + i] != ' ') {
            return 0;
        }
    }

    return 1;
}

int checkKeyword(char* str) {
    char list[12][20] = {
        {"break"}, {"case"}, {"continue"}, {"do"}, {"else"}, {"for"}, {"goto"}, {"if"}, {"return"}, {"switch"}, {"while"}, {'\0'}
    };

    for (int i = 0; list[i][0] != 0; i++) {
        if (cmpstr(str, list[i]))
            return 1;
    }

    return 0;
}