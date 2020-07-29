//
// Created by ym500 on 2019/11/16.
//

#include "function_table.h"
#include <iostream>
using namespace std;
function_table_element::function_table_element(string name, string type, bool has_return) {
    this->name = name;
    this->type = type;
    this->has_return = has_return;
}

function_table::function_table() {
}

function_table_element function_table::get(int i) {
    return this->function_table1.at(i);
}

int function_table::find_loc(string name) {
    for (int i = 0; i < this->function_table1.size(); i++) {
        if(this->get(i).name == name){
            return i;
        }
    };
    return -1;
}

int function_table::get_func_table_size() {
    return this->function_table1.size();
}

void function_table::add(string name, string type, bool has_return) {
    this->function_table1.push_back(function_table_element(name,type,has_return));
}

void function_table::add_param(string function_name,string type,string ident) {
    this->function_table1.at(find_loc(function_name)).param_list.push_back(pair<string,string>(type,ident));
}

string function_table::get_func_type(string name) {
    return this->get(find_loc(name)).type;
}

bool function_table::get_func_has_return(string name) {
    return this->get(find_loc(name)).has_return;
}

int function_table::get_func_param_count(string name) {
    return this->get(find_loc(name)).param_list.size();
}