//
// Created by ym500 on 2019/11/19.
//

#ifndef UNTITLED_INSTRUCTION_H
#define UNTITLED_INSTRUCTION_H
#include<string>
using namespace std;

class instruction{
public:
    string instruction_name;
    string first_pos;
    string second_pos;
    string third_pos;
    instruction(string label);
    instruction(string name,string reg_name);
    instruction(string name,string reg_name,string s_value);
    instruction(string name,string first,string second,string third);
};
void push_to_instruction_vector(string label);
void push_to_instruction_vector(string name,string label);
void push_to_instruction_vector(string name,string reg_name,string s_value);
void push_to_instruction_vector(string name,string first,string second,string third);
#endif //UNTITLED_INSTRUCTION_H
