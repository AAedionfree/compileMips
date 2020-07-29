//
// Created by ym500 on 2019/10/17.
//
#include "split.h"
#include <string>
#include <vector>
#include<string.h>

using namespace std;

char CONST_STRING[][20] = {"const", "int", "char", "void", "main", "if", "else", "do", "while", "for", "scanf",
                           "printf", "return"};
char CONST_RETURN[][20] = {"CONSTTK", "INTTK", "CHARTK", "VOIDTK", "MAINTK", "IFTK", "ELSETK", "DOTK", "WHILETK",
                           "FORTK", "SCANFTK", "PRINTFTK", "RETURNTK"};
char CONST_LEN_1[][20] = {"<", ">", "=", "+", "-", "*", "/", ";", ",", "(", ")", "[", "]", "{", "}"};
char CONST_LEN_1_RETURN[][20] = {"LSS", "GRE", "ASSIGN", "PLUS", "MINU", "MULT", "DIV", "SEMICN", "COMMA", "LPARENT",
                                 "RPARENT", "LBRACK", "RBRACK", "LBRACE", "RBRACE"};
char CONST_LEN_2[][20] = {"<=", ">=", "==", "!="};
char CONST_LEN_2_RETURN[][20] = {"LEQ", "GEQ", "EQL", "NEQ"};


char buffer[BUFFERSIZE];
int codebuffer_pos;
int buffer_pos;

extern vector<pair<string, string> > vector_buffer;
extern char codebuffer[MAXSIZE];

void push_back_to_buffer(string string1, string string2) {
    vector_buffer.push_back(pair<string, string>(string1, string2));
}

bool is_alpha(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_';
}

bool is_digit(char ch) {
    return ch >= '0' && ch <= '9';
}

bool is_trim(char ch) {
    return ch == '\n' || ch == '\r' || ch == ' ' || ch == '\t' || ch == '\0';
}

void recall() {
    codebuffer_pos--;
}

void buffer_pop() {
    buffer_pos--;
}

char codebuffer_getchar() {
    return codebuffer[codebuffer_pos++];
}

void trim() {
    char ch = codebuffer_getchar();
    while (is_trim(ch)) {
        ch = codebuffer_getchar();
    }
    recall();
}

void clearbuffer() {
    int i;
    for (i = 0; i < BUFFERSIZE; i++) {
        buffer[i] = 0;
    }
    buffer_pos = 0;
}

void buffer_putchar(char ch) {
    buffer[buffer_pos++] = ch;
}

bool split() {
    trim();
    clearbuffer();
    char ch = codebuffer_getchar();
    if (ch == EOF) {
        return false;
    }
    buffer_putchar(ch);
    if (is_alpha(ch)) {
        char ch_temp = codebuffer_getchar();
        while (is_alpha(ch_temp) || is_digit(ch_temp)) {
            buffer_putchar(ch_temp);
            ch_temp = codebuffer_getchar();
        }
        recall();
        if (!is_const_string()) {
            push_back_to_buffer(string("IDENFR"), string(buffer));
        }
    } else if (is_digit(ch)) {
        if (ch == '0') {
            push_back_to_buffer(string("INTCON"), string(buffer));
            return true;
        }
        char ch_temp = codebuffer_getchar();
        while (is_digit(ch_temp)) {
            buffer_putchar(ch_temp);
            ch_temp = codebuffer_getchar();
        }
        buffer[buffer_pos] = '\0';
        recall();
        push_back_to_buffer(string("INTCON"), string(buffer));
    } else {
        if (is_const_char_len_1()) {
            return true;
        }
        char ch_temp;
        if (ch == 39 || ch == 34) {
            buffer_pop();
            ch_temp = codebuffer_getchar();
            while (ch_temp != ch) {
                buffer_putchar(ch_temp);
                ch_temp = codebuffer_getchar();
            }
            buffer[buffer_pos] = '\0';
            if (ch == 34) {
                push_back_to_buffer(string("STRCON"), string(buffer));
                return true;
            } else if (ch == 39) {
                push_back_to_buffer(string("CHARCON"), string(buffer));
                return true;
            }
        }
        ch_temp = codebuffer_getchar();
        buffer_putchar(ch_temp);
        if (!is_const_char_len_2()) {
            printf("error!!!!!");
        }
    }
    return true;
}


bool is_const_string() {
    int i = 0;
    bool result = false;
    buffer[buffer_pos] = '\0';
    for (i = 0; i < CONST_STRING_NUMBER; i++) {
        if (strcmp(buffer, CONST_STRING[i]) == 0) {
            push_back_to_buffer(string(CONST_RETURN[i]), string(CONST_STRING[i]));
            result = true;
            break;
        }
    }
    return result;
}

bool is_const_char_len_1() {
    int i = 0;
    bool result = false;
    buffer[buffer_pos] = '\0';
    for (i = 0; i < CONST_CHAR_LEN_1_NUMBER; i++) {
        if (strcmp(buffer, CONST_LEN_1[i]) == 0) {
            if (i == 0 || i == 1 || i == 2) {
                char ch = codebuffer_getchar();
                if (ch == '=') {
                    push_back_to_buffer(string(CONST_LEN_2_RETURN[i]), string(CONST_LEN_2[i]));
                    result = true;
                    break;
                } else {
                    recall();
                }
            }
            push_back_to_buffer(string(CONST_LEN_1_RETURN[i]), string(CONST_LEN_1[i]));
            result = true;
            break;
        }
    }
    return result;
}

bool is_const_char_len_2() {
    int i = 0;
    bool result = false;
    buffer[buffer_pos] = '\0';
    for (i = 0; i < CONST_CHAR_LEN_2_NUMBER; i++) {
        if (strcmp(buffer, CONST_LEN_2[i]) == 0) {
            push_back_to_buffer(string(CONST_LEN_2_RETURN[i]), string(CONST_LEN_2[i]));
            result = true;
            break;
        }
    }
    return result;
}

