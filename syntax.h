//
// Created by ym500 on 2019/10/17.
//

#ifndef UNTITLED_SYNTAX_H
#define UNTITLED_SYNTAX_H
#include<string>
#include<utility>
using namespace std;

class value_midcode{
public:
    bool is_const;
    int const_value;
    string temp_name;
    value_midcode(bool is_const=false,int const_value=0,string temp_name=string("#NULL"));
};
pair<string,string> getsym();

void const_declare();

void const_define();

void var_declare();

void var_define(string type);

void head_of_declaration();

void param_list();

void complex_sentence();

void has_return_func();

int check_int();

int check_no_signal_int();

void no_return_func();

void sentence_list();

void sentence();

value_midcode factor();

value_midcode item();

value_midcode express();

void condition();

void if_sentence();

void loop_sentence();

void value_param_list(string func_name);

value_midcode invoke_has_return_func();

void invoke_no_return_func();

void assgin_sentence();

int step();

void read_sentence();

void write_sentence();

void return_sentence();

void main_check();

void process();
#endif //UNTITLED_SYNTAX_H
