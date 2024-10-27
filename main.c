#include <stdio.h>
#include "func.c"
#include "macro.h"


char str[MAX] = {0, };

int main() {
START
  FILE* file;

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
  char parsed[MAX][MAX] = {0, };
  int tag[MAX] = {0, };

START
  int row, col = 0;

  for (int i = 0; str[i] != 0;) {
    if (checkStr(str[i])) {
      tag[row] = STR;
      parsed[row][col++] = '\\'; 
      parsed[row][col++] = str[i++];

      while (checkStr(str[i]) != 1) {
        parsed[row][col++] = str[i];

        if (str[i] == '\\') {
          parsed[row][col++] = '\\';
          i++;

          if (checkStr(str[i])) {
            parsed[row][col++] = '\\';
            parsed[row][col++] = str[i];
          }
          else if (checkEscape(str[i]))
            parsed[row][col++] = str[i];
          else if (str[i] == '\\') {
            parsed[row][col++] = '\\';
            parsed[row][col++] = str[i];
          }
        }

        i++;
      }

      parsed[row][col++] = '\\';
      parsed[row][col] = str[i++];
    }
    else if (checkInt(str[i])) {
      tag[row] = INT;
      
      while (checkInt(str[i])) {
        parsed[row][col++] = str[i++];
      }
    }
    else if (checkEtc(str[i])) {
      tag[row] = ETC;

      while (checkEtc(str[i])) {
        parsed[row][col++] = str[i++];
      }
    }
    else if (checkIdent(str[i])) {
      tag[row] = IDENT;
      
      while (checkIdent(str[i])) {
        parsed[row][col++] = str[i++];
      }
    }
    else if (checkParen(str[i])) {
      tag[row] = PAREN;
      parsed[row][col++] = str[i++];
    }
    else if (checkDivider(str[i])) {
      if (checkIndent(str, i)) {
        tag[row] = INDENT;

        for (int j = 0; j < INDENT_SIZE; j++) {
          parsed[row][col++] = str[i++];
        }
      }
      else {
        tag[row] = DIV;
        parsed[row][col++] = str[i++];
      }
    }

    row++;
    col = 0;
  }
END

  for (int i = 0; parsed[i][0] != 0; i++) {
    if (checkKeyword0(parsed[i]))
      tag[i] = KEYWORD0;
    else if (checkKeyword1(parsed[i]))
      tag[i] = KEYWORD1;
    else if (parsed[i + 1][0] == '(')
      tag[i] = FUNC;
  }

  FILE* file;
  file = fopen("result.js", "w");

  fprintf(file, "<div>\n");
  for (int i = 0; tag[i] != ZERO; i++) {
    if (tag[i] == IDENT)
      fprintf(file, "<span className=\"code-cyan\">{\"%s\"}</span>", parsed[i]);

    else if (tag[i] == FUNC)
      fprintf(file, "<span className=\"code-lightyellow\">{\"%s\"}</span>", parsed[i]);

    else if (tag[i] == INT)
      fprintf(file, "<span className=\"code-lightgreen\">{\"%s\"}</span>", parsed[i]);

    else if (tag[i] == STR)
      fprintf(file, "<span className=\"code-orange\">{\"%s\"}</span>", parsed[i]);

    else if (tag[i] == PAREN)
      fprintf(file, "<span className=\"code-yellow\">{\"%s\"}</span>", parsed[i]);

    else if (tag[i] == ETC)
      fprintf(file, "<span className=\"code-white\">{\"%s\"}</span>", parsed[i]);

    else if (tag[i] == KEYWORD0)
      fprintf(file, "<span className=\"code-blue\">{\"%s\"}</span>", parsed[i]);

    else if (tag[i] == KEYWORD1)
      fprintf(file, "<span className=\"code-magenta\">{\"%s\"}</span>", parsed[i]);

    else if (tag[i] == DIV) {
      if (parsed[i][0] == ' ')
        fprintf(file, "%s", parsed[i]);
      else if (parsed[i][0] == '\n')
        fprintf(file, "<br></br>\n");
    }

    else if (tag[i] == INDENT)
      fprintf(file, "<span>&nbsp;</span>");
  }
  fprintf(file, "\n</div>");

  fclose(file);
END

  return 0;
}