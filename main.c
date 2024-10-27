#include <stdio.h>
#include "func.c"
#include "macro.h"

#define START {
#define END }


char str[MAX] = {0, };

int main() {
START
  FILE* file;

// 파일 열기
START
  char fname[MAX];

  printf("File name: ");
  scanf("%s", (char*)(fname));
  file = fopen(fname, "r");

  if (file == NULL) {
    printf("No file name \'%s\'.", fname);
    return 0;
  }
END

// 읽기
  for (int i = 0; ; i++) {
    char temp = fgetc(file);

    if (feof(file) != 0)
      break;
    else
      str[i] = temp;
  }

  fclose(file);
  printf("%s", str);
END

START
  char parsed[MAX][0xff] = {0, };
  int tag[MAX] = {0, };

START
  int row, col = 0;

// 전체적으로 설정
  for (int i = 0; str[i] != 0;) {
    if (isalpha(str[i]) || str[i] == '_') {
      tag[row] = IDENT;
      parsed[row][col++] = str[i++];

      while (isalpha(str[i]) || str[i] == '_' || isdigit(str[i])) {
        parsed[row][col++] = str[i++];
      }
    }

    else if (isdigit(str[i])) {
      tag[row] = INT;

      if (str[i] == '0') {
        parsed[row][col++] = str[i++];

        if (str[i] == 'b') {
          parsed[row][col++] = str[i++];

          while (isdigit(str[i])) {
            parsed[row][col++] = str[i++];
          }
        }
        
        else if (str[i] == 'x') {
          while (isalpha(str[i]) || isdigit(str[i])) {
            parsed[row][col++] = str[i++];
          }
        }
      }
      
      while (isdigit(str[i])) {
        parsed[row][col++] = str[i++];
      }
    }

    else if (ispunct(str[i])) {
      if (isparen(str[i])) {
        tag[row] = PAREN;
        parsed[row][col++] = str[i++];
      }

      else if (str[i] == '\'' || str[i] == '\"') {
        tag[row] = STR;
        parsed[row][col++] = '\\';
        parsed[row][col++] = str[i++];

        while (str[i] != '\'' && str[i] != '\"') {
          if (str[i] == '\\') {
            row++;
            col = 0;
            tag[row] = ESCAPE;

            parsed[row][col++] = '\\';
            parsed[row][col++] = str[i++];

            if (str[i] == '\'' || str[i] == '\"' || str[i] == '\\') {
              parsed[row][col++] = '\\';
              parsed[row][col++] = str[i++];
            }
            else {
              parsed[row][col++] = str[i++];
            }

            row++;
            col = 0;
            tag[row] = STR;
          }
          else
            parsed[row][col++] = str[i++];
        }
        
        parsed[row][col++] = '\\';
        parsed[row][col++] = str[i++];
      }

      else if (str[i] == '/') {
        if (str[i + 1] == '/') {
          tag[row] = COMMENT;

          while (str[i] != '\n') {
            parsed[row][col++] = str[i++];
          }
        }
        else {
          tag[row] = ETC;
          parsed[row][col++] = str[i++];
        }
      }

      else {
        tag[row] = ETC;
        parsed[row][col++] = str[i++];
      }
    }

    else if (str[i] == ' ') {
      if (isindent(str, i)) {
        tag[row] = INDENT;
        
        for (int j = 0; j < INDENT_SIZE; j++) {
          parsed[row][col++] = str[i++];
        }
      }

      else {
        tag[row] = SPACE;
        parsed[row][col++] = str[i++];
      }
    }

    else if (str[i] == '\n') {
      tag[row] = BREAK;
      parsed[row][col++] = str[i++];
    }

    else if (str[i] == '\t') {
      tag[row] = INDENT;
      parsed[row][col++] = str[i++];
    }

    row++;
    col = 0;
  }
END

// 자세하게 설정
  for (int i = 0; tag[i] != 0 && i < MAX; i++) {
    if (tag[i] == ETC && parsed[i][0] == '#') {
      tag[i++] = KEYWORD;
      tag[i++] = KEYWORD;

      for (; tag[i] == SPACE; i++);

      if (tag[i] == ETC && parsed[i][0] == '<') {
        while (!(tag[i] == ETC && parsed[i][0] == '>')) {
          tag[i++] = STR;
        }

        tag[i] = STR;
      }
    }

    else if (checkKeyword(parsed[i]))
      tag[i] = KEYWORD;

    else if (tag[i] == IDENT) {
      int j = i + 1;

      for (; tag[j] == SPACE; j++);

      if (tag[j] == PAREN && parsed[j][0] == '(')
        tag[i] = FUNC;
    }
  }

// 쓰기
  FILE* file;
  file = fopen("result.js", "w");

  fprintf(file, "<div>\n");

  for (int i = 0; tag[i] != ZERO; i++) {
    if (tag[i] == IDENT)
      fprintf(file, "<span className=\"code-white\">{\"%s\"}</span>", parsed[i]);

    else if (tag[i] == FUNC)
      fprintf(file, "<span className=\"code-purple\">{\"%s\"}</span>", parsed[i]);

    else if (tag[i] == INT)
      fprintf(file, "<span className=\"code-lightgreen\">{\"%s\"}</span>", parsed[i]);

    else if (tag[i] == STR)
      fprintf(file, "<span className=\"code-orange\">{\"%s\"}</span>", parsed[i]);

    else if (tag[i] == PAREN)
      fprintf(file, "<span className=\"code-blue\">{\"%s\"}</span>", parsed[i]);

    else if (tag[i] == ETC)
      fprintf(file, "<span className=\"code-lightyellow\">{\"%s\"}</span>", parsed[i]);

    else if (tag[i] == KEYWORD)
      fprintf(file, "<span className=\"code-magenta\">{\"%s\"}</span>", parsed[i]);

    else if (tag[i] == SPACE)
      fprintf(file, "%s", parsed[i]);
    
    else if (tag[i] == INDENT)
      fprintf(file, "<span>&nbsp;&nbsp;</span>");

    else if (tag[i] == BREAK)
      fprintf(file, "<br></br>\n");

    else if (tag[i] == COMMENT)
      fprintf(file, "<span className=\"code-gray\">{\"%s\"}</span>", parsed[i]);

    else if (tag[i] == ESCAPE)
      fprintf(file, "<span className=\"code-lightyellow\">{\"%s\"}</span>", parsed[i]);
  }

  fprintf(file, "\n</div>");

  fclose(file);
END

  return 0;
}