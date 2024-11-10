#include <stdio.h>
#include "func.c"
#include "macro.h"

#define START {
#define END }


char str[MAX] = {0, };

int main() {
START
    FILE* file;

// Open Source File
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

// Read Source File
    for (int i = 0; feof(file) == 0; i++)
        str[i] = fgetc(file);

    fclose(file);
    printf("%s", str);
END


START
    char parsed[MAX][0xff] = {0, };
    int tag[MAX] = {0, };

START
    int row, col = 0;

// Parse Source and Set Tag
    for (int i = 0; str[i] != 0;) {
        if (isalpha(str[i]) || str[i] == '_') { // Parse Identifier
            tag[row] = IDENT;
            while (isalpha(str[i]) || str[i] == '_' || isdigit(str[i]))
                parsed[row][col++] = str[i++];
        }
        
        else if (isdigit(str[i])) { // Parse Integer
            tag[row] = INT;
            if (str[i] == '0' && (str[i + 1] == 'b' || str[i + 1] == 'x')) { // Binary and Hexademical
                while (isalpha(str[i]) || isdigit(str[i]))
                    parsed[row][col++] = str[i++];
            }
            else { // Decimal
                while (isdigit(str[i]))
                    parsed[row][col++] = str[i++];
            }
        }

        
        else if (isparen(str[i])) { // Parse Parenthesis
            tag[row] = PAREN;
            parsed[row][col++] = str[i++];
        }

        else if (str[i] == '\'' || str[i] == '\"') { // Parse String
            tag[row] = STR;
            parsed[row][col++] = '\\';
            parsed[row][col++] = str[i++];

            while (str[i] != '\'' && str[i] != '\"') {
                if (str[i] == '\\') { // Escape Sequence
                    row++;
                    col = 0;
                    tag[row] = ESCAPE;

                    parsed[row][col++] = '\\';
                    parsed[row][col++] = str[i++];

                    if (str[i] == '\'' || str[i] == '\"' || str[i] == '\\') { // Quote and Backslash
                        parsed[row][col++] = '\\';
                        parsed[row][col++] = str[i++];
                    }
                    else { // Other
                        parsed[row][col++] = str[i++];
                    }

                    row++;
                    col = 0;
                    tag[row] = STR;
                }
                else {
                    parsed[row][col++] = str[i++];
                }
            }

            parsed[row][col++] = '\\';
            parsed[row][col++] = str[i++];
        }

        else if (str[i] == '/' && str[i + 1] == '/') { // Parse Comment
            tag[row] = COMMENT;
            while (str[i] != '\n' && str[i] != '\0') 
                parsed[row][col++] = str[i++];
        }

        else if (ispunct(str[i])) { // Parse other punctuation
            tag[row] = ETC;
            parsed[row][col++] = str[i++];
        }

        else if (str[i] == ' ') {
            if (isindent(str, i)) { // Parse Indentation
                tag[row] = INDENT;
                for (int j = 0; j < INDENT_SIZE; j++)
                    i++;
            }
            else { // Parse Space
                tag[row] = SPACE;
                i++;
            }
        }

        else if (str[i] == '\n') { // Parse Line Break
            tag[row] = BREAK;
            i++;
        }

        else if (str[i] == '\t') { // Parse Tab
            tag[row] = INDENT;
            i++;
        }

        else
            i++;

        row++;
        col = 0;
    }
END


// Set Tag More Precisely
    for (int i = 0; tag[i] != 0 && i < MAX; i++) {
        if (tag[i] == ETC && parsed[i][0] == '#') { // Preprocesser
            tag[i] = KEYWORD;
            for (i += 1; tag[i] == SPACE; i++);
            tag[i] = KEYWORD;

            for (; tag[i] != BREAK && tag[i] != ZERO; i++) {
                if (tag[i] == STR || tag[i] == ETC)
                    tag[i] = IDENT;
            }
        }

        else if (checkKeyword(parsed[i])) // C Keyword
            tag[i] = KEYWORD;

        else if (tag[i] == IDENT) { // Function
            int j = i + 1;

            for (; tag[j] == SPACE; j++);

            if (tag[j] == PAREN && parsed[j][0] == '(')
                tag[i] = FUNC;
        }
    }

// Write to Result File
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
            fprintf(file, "&nbsp;");
        
        else if (tag[i] == INDENT)
            fprintf(file, "&nbsp;&nbsp;");

        else if (tag[i] == BREAK)
            fprintf(file, "<br></br>\n");

        else if (tag[i] == COMMENT)
            fprintf(file, "<span className=\"code-gray\">{\"%s\"}</span>", parsed[i]);

        else if (tag[i] == ESCAPE)
            fprintf(file, "<span className=\"code-yellow\">{\"%s\"}</span>", parsed[i]);
    }

    fprintf(file, "\n</div>");

    fclose(file);
END

    return 0;
}