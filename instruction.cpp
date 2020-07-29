//
// Created by ym500 on 2019/11/19.
//

#include "instruction.h"
#include "string_value.h"
#include <vector>

//using namespace std;
vector<instruction> instruction_vector;

instruction::instruction(string name) {
    this->instruction_name = name;
    this->first_pos = null_midcode;
    this->second_pos = null_midcode;
    this->third_pos = null_midcode;
}

instruction::instruction(string name, string first) {
    this->instruction_name = name;
    this->first_pos = first;
    this->second_pos = null_midcode;
    this->third_pos = null_midcode;
}

instruction::instruction(string name, string first, string second) {
    this->instruction_name = name;
    this->first_pos = first;
    this->second_pos = second;
    this->third_pos = null_midcode;
}

instruction::instruction(string name, string first, string second, string third) {
    this->instruction_name = name;
    this->first_pos = first;
    this->second_pos = second;
    this->third_pos = third;
}

void push_to_instruction_vector(string name) {
    instruction_vector.push_back(instruction(name));
}

void push_to_instruction_vector(string name, string first) {
    instruction_vector.push_back(instruction(name, first));
}

void push_to_instruction_vector(string name, string first, string second) {
    instruction_vector.push_back(instruction(name, first, second));
}

void push_to_instruction_vector(string name, string first, string second, string thrid) {
    instruction_vector.push_back(instruction(name, first, second, thrid));
}

