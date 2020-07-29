//
// Created by ym500 on 2019/11/17.
//

#include <vector>
#include "midcode.h"

using namespace std;

extern vector<midcode> midcode_vector;

midcode::midcode(string first, string second, string third, string fourth) {
    this->first_pos = first;
    this->second_pos = second;
    this->third_pos = third;
    this->fourth_pos = fourth;
}

string midcode::get_first_pos() {
    return this->first_pos;
}

string midcode::get_second_pos() {
    return this->second_pos;
}

string midcode::get_third_pos() {
    return this->third_pos;
}

string midcode::get_fourth_pos() {
    return this->fourth_pos;
}

void add_MidcodeToVector(string first, string second, string third, string fourth) {
    midcode_vector.push_back(midcode(first, second, third, fourth));
}