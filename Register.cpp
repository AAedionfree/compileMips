//
// Created by ym500 on 2019/11/19.
//

#include "Register.h"
#include <vector>

using namespace std;
extern vector<Register> GRF;
int t = 0;

Register::Register(string name, int value, int save_var_index, int use_time, bool is_used) {
    this->name = name;
    this->value = value;
    this->save_var_table_pos = save_var_index;
    this->use_time = use_time;
    this->is_used = is_used;
}

string Register::get_name() {
    return this->name;
}

string Register::get_value_name(){
    return this->value_name;
}

string Register::get_func_name(){
    return this->func_name;
}
int Register::get_value() {
    return this->value;
}

int Register::get_save_var_index() {
    return this->save_var_table_pos;
}

int Register::get_use_time() {
    return this->use_time;
}

bool Register::get_is_used() {
    return this->is_used;
}

void Register::set_is_unused(){
    this->is_used = false;
}

void Register::set_register(string value_name,string func_name) {
    this->value = 0;
    this->func_name = func_name;
    this->use_time = t++;
    this->is_used = true;
    this->value_name = value_name;
}

void Register::set_register(string value_name) {
    this->value_name = value_name;
    this->is_used = true;
    this->use_time = t++;
}
void Register::use_time_plus(){
    this->use_time = this->use_time + 1;
}

int get_register_index(string register_name) {
    for (int i = 0; i < GRF.size(); i++) {
        if(GRF.at(i).get_name() == register_name){
            return i;
        }
    }
    return -1;
}
