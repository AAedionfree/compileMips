//
// Created by ym500 on 2019/10/17.
//

#ifndef UNTITLED_SPLIT_H
#define UNTITLED_SPLIT_H

#include<string>
using namespace std;
#define MAXSIZE 100000
#define BUFFERSIZE 200
#define CONST_STRING_NUMBER 13
#define CONST_CHAR_LEN_1_NUMBER 15
#define CONST_CHAR_LEN_2_NUMBER 4
bool split();

bool is_const_string();

bool is_const_char_len_1();

bool is_const_char_len_2();

void push_back_to_buffer(string string1, string string2);
#endif //UNTITLED_SPLIT_H
