#include "macro.h"

int cmpstr(char* str1, char* str2) {
  for (int i = 0; str1[i] != 0 || str2[i] != 0; i++) {
    if (str1[i] != str2[i])
      return 0;
  }

  return 1;
}

int checkParen(char c) {
  char list[] = "()[]{}";

  for (int i = 0; list[i] != 0; i++) {
    if (c == list[i])
      return 1;
  }

  return 0;
}

int checkEtc(char c) {
  char list[] = "+-*/%&|,.=!<>;:";

  for (int i = 0; list[i] != 0; i++) {
    if (c == list[i])
      return 1;
  }

  return 0;
}

int checkStr(char c) {
  char list[] = "\"\'";

  for (int i = 0; list[i] != 0; i++) {
    if (c == list[i])
      return 1;
  }

  return 0;
}

int checkInt(char c) {
  char list[] = "0123456789";

  for (int i = 0; list[i] != 0; i++) {
    if (c == list[i])
      return 1;
  }

  return 0;
}

int checkDivider(char c) {
  char list[] = " \n";

  for (int i = 0; list[i] != 0; i++) {
    if (c == list[i])
      return 1;
  }

  return 0;
}

int checkIdent(char c) {
  if (checkParen(c) || checkEtc(c) || checkStr(c) || checkInt(c) || checkDivider(c))
    return 0;
  else
    return 1;
}

int checkIndent(char* str, int index) {
  for (int i = 0; i < INDENT_SIZE; i++) {
    if (str[index + i] != ' ')
      return 0;
  }

  return 1;
}

int checkKeyword0(char* str) {
  char list[20][20] = {
    {"char"}, {"short"}, {"int"}, {"long"}, {"unsigned"}, {"struct"}, {"extern"}, {"static"}, {"const"}, {"double"}, {"float"}, {"enum"}, {"inline"}, {"signed"}, {"sizeof"}, {"typedef"}, {"union"}, {"void"}, {"volatile"}, {'\0'}
  };
  
  for (int i = 0; list[i][0] != 0; i++) {
    if (cmpstr(str, list[i]))
      return 1;
  }

  return 0;
}

int checkKeyword1(char* str) {
  char list[12][20] = {
    {"break"}, {"case"}, {"continue"}, {"do"}, {"else"}, {"for"}, {"goto"}, {"if"}, {"return"}, {"switch"}, {"while"}, {'\0'}
  };
  
  for (int i = 0; list[i][0] != 0; i++) {
    if (cmpstr(str, list[i]))
      return 1;
  }

  if (str[0] == '#')
    return 1;

  return 0;
}

int checkEscape(char c) {
  char list[] = "nrtbf";
  
  for (int i = 0; list[i] != 0; i++) {
    if (c == list[i])
      return 1;
  }

  return 0;
}