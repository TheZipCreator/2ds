#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#define WIDTH 0xFF
#define HEIGHT 0xFF
#include "error.h"
#include "2ds.h"
#include "simple_fileio.h"
#include "stringmanip.h"
#define pushx(data) pushx_(stack, head, data, line)
#define pushy(data) pushy_(stack, head, data, line)
#define popx() popx_(stack, head, line)
#define popy() popy_(stack, head, line)
#define streq(a, b) (strcmp(a, b) == 0)
#define MAX_TOKEN_LENGTH 1024

struct Token {
    char* value;
    int line;
    int length;
    struct Token* next;
    struct Token* prev;
};

struct Token *tokenize(char* file);
struct Token *appendToken(struct Token *token, char* value, int line);
struct Token *firstTokenInLine(struct Token *token, int line);
struct Token *getPrev(struct Token *head, struct Token *curr);

int main(int argc, char** argv) {
    if(argc == 3) {
        if(streq(argv[2], "-minimize")) {
            char* file = loadFile(argv[1]);
            struct Token* curr = tokenize(file);
            while(curr != NULL) {
                printf("%s ", curr->value);
                curr = curr->next;
            }
            printf("\n");
            return 0;
        } else {
            printf("Usage: %s <filename> [-minimize]\n", argv[0]);
            return 1;
        }
    }
    if(argc != 2) {
        printf("Usage: %s <filename> [-minimize]\n", argv[0]);
        printf("-minimize will minimize the file to a single line and then print it back out.\n");
        return 1;
    }
    //Initialize stack
    byte **stack = malloc(WIDTH * sizeof(byte*));
    for(int i = 0; i < WIDTH; i++) {
        stack[i] = malloc(HEIGHT * sizeof(byte));
    }
    Point *head = malloc(sizeof(Point));
    head->x = 0;
    head->y = 0;
    //Load file
    char* file = loadFile(argv[1]);
    //Run tokenizer
    struct Token *curr = tokenize(file);
    struct Token *headToken = curr;
    //Interpret tokens
    byte a = 0;
    byte b = 0;
    while(curr != NULL) {
        char* value = malloc(curr->length + 1);
        strncpy(value, curr->value, curr->length);
        value = toLowerCase(value);
        int line = curr->line;
        char* num = substring(value, 0, strlen(value)-1);
        if(value[strlen(value)-1] == 'x' && isIntegerString(num)) {
            pushx(atoi(num));
        } else if(value[strlen(value)-1] == 'y' && isIntegerString(num)) {
            pushy(atoi(num));
        } else if(streq(value, "xa")) {
            a = popx();
        } else if(streq(value, "ya")) {
            a = popy();
        } else if(streq(value, "xb")) {
            b = popx();
        } else if(streq(value, "yb")) {
            b = popy();
        } else if(streq(value, "ax")) {
            pushx(a);
        } else if(streq(value, "ay")) {
            pushy(a);
        } else if(streq(value, "bx")) {
            pushx(b);
        } else if(streq(value, "by")) {
            pushy(b);
        } else if(streq(value, "+")) {
            a += b;
        } else if(streq(value, "-")) {
            a -= b;
        } else if(streq(value, "*")) {
            a *= b;
        } else if(streq(value, "/")) {
            a /= b;
        } else if(streq(value, "x.")) {
            printf("%c", popx());
        } else if(streq(value, "y.")) {
            printf("%c", popy());
        } else if(streq(value, "==")) {
            a = (a == b);
        } else if(streq(value, ">")) {
            a = (a > b);
        } else if(streq(value, "<")) {
            a = (a < b);
        } else if(streq(value, ">=")) {
            a = (a >= b);
        } else if(streq(value, "<=")) {
            a = (a <= b);
        } else if(streq(value, "!=")) {
            a = (a != b);
        } else if(streq(value, "!")) {
            a = !a;
        } else if(streq(value, "&&")) {
            a = (a && b);
        } else if(streq(value, "||")) {
            a = (a || b);
        } else if(streq(value, "if")) {
            if(a == 0) {
                //Skip to corresponding fi
                int fi = 1;
                while(fi > 0) {
                    curr = curr->next;
                    if(curr == NULL) {
                        error("Unmatched if", line);
                    }
                    value = curr->value;
                    value = toLowerCase(value);
                    if(streq(value, "if")) {
                        fi++;
                    } else if(streq(value, "fi")) {
                        fi--;
                    }
                }
            }
        } else if(streq(value, "while")) {
            if(a == 0) {
                //Skip to corresponding elihw
                int elihw = 1;
                while(elihw > 0) {
                    curr = curr->next;
                    if(curr == NULL) {
                        error("Unmatched while", line);
                    }
                    value = curr->value;
                    value = toLowerCase(value);
                    if(streq(value, "while")) {
                        elihw++;
                    } else if(streq(value, "elihw")) {
                        elihw--;
                    }
                }
            }
        }
        else if(streq(value, "fi")) {
            //end of if - do nothing
        } else if(streq(value, "elihw")) {
            if(a != 0) {
                //go to the corresponding while
                int whileCount = 1;
                while(whileCount > 0) {
                    curr = curr->prev;
                    if(curr == NULL) {
                        error("Unmatched elihw", line);
                    }
                    value = curr->value;
                    value = toLowerCase(value);
                    if(streq(value, "while")) {
                        whileCount--;
                    } else if(streq(value, "elihw")) {
                        whileCount++;
                    }
                }
            }
        } else if(streq(value, ".x")) {
            pushx(getch());
        } else if(streq(value, ".y")) {
            pushy(getch());
        } else if(streq(value, "swap")) {
            int temp = a;
            a = b;
            b = temp;
        } else if(value[0] == '\'' && strlen(value) == 3) {
            char c = curr->value[1];
            if(value[2] == 'x') {
                pushx(c);
            } else if(value[2] == 'y') {
                pushy(c);
            } else {
                char msg[100];
                sprintf(msg, "Unknown token: %s", value);
                error(msg, line);
            }
        }
        else if(!streq(value, "")){
            char msg[100];
            sprintf(msg, "Unknown token: %s", value);
            error(msg, line);
        }
        curr = curr->next;
    }
    return 0;
}

struct Token *tokenize(char* file) {
    struct Token *curr = malloc(sizeof(struct Token));
    struct Token *head = curr;
    char* str = malloc(MAX_TOKEN_LENGTH);
    int line = 1;
    int end = 0;
    int comment = 0;
    for(int i = 0; i < strlen(file); i++) {
        char c = file[i];
        if(comment) {
            if(c == '\n') {
                line++;
                comment = 0;
            }
        } else {
            switch(c) {
                case '\n':
                    curr = appendToken(curr, substring(str, 0, end), line);
                    end = 0;
                    line++;
                    break;
                case ' ': //end of token
                    curr = appendToken(curr, substring(str, 0, end), line);
                    end = 0;
                    break;
                case '\t': //end of token
                    curr = appendToken(curr, substring(str, 0, end), line);
                    end = 0;
                    break;
                case '#':
                    comment = 1;
                    break;
                default:
                    str[end] = c;
                    end++;
                    break;
            }
        }
    }
    curr = appendToken(curr, substring(str, 0, end), line);
    curr->value = "";
    curr->line = line;
    curr->length = 0;
    curr->next = NULL;
    free(str);
    return head;
}

struct Token *getPrev(struct Token *head, struct Token *curr) {
    struct Token *prev = head;
    while(prev->next != curr) {
        prev = prev->next;
    }
    return prev;
}

struct Token *appendToken(struct Token *token, char* value, int line) {
    if(strlen(value) == 0) return token; //Don't add empty tokens
    token->next = malloc(sizeof(struct Token));
    token->next->prev = token;
    token->length = strlen(value);
    token->value = value;
    token->line = line;
    return token->next;
}

struct Token *firstTokenInLine(struct Token *token, int line) {
    while(token != NULL && token->line != line) {
        token = token->next;
    }
    return token;
}