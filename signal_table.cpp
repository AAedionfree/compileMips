//
// Created by ym500 on 2019/11/16.
//

#include"siganl_table.h"

signal_table_element::signal_table_element(string name, string kind, string type, string belong_function,
                                           bool is_global, bool is_array,
                                           int array_size) {
    this->name = name;
    this->kind = kind;
    this->type = type;
    this->belong_function = belong_function;
    this->is_global = is_global;
    this->is_array = is_array;
    this->array_size = array_size;
    this->value = 0;
    this->offset = -1;
}

signal_table_element::signal_table_element(string name, string kind, string type, string belong_function,
                                           bool is_global, bool is_array,
                                           int array_size, int value, int offset) {
    this->name = name;
    this->kind = kind;
    this->type = type;
    this->belong_function = belong_function;
    this->is_global = is_global;
    this->is_array = is_array;
    this->array_size = array_size;
    this->value = value;
    this->offset = offset;
}


signal_table::signal_table() {
    this->size = 0;
}

int signal_table::get_size() {
    return this->size;
}

signal_table_element signal_table::get(int i) {
    return this->signal_table1.at(i);
}

void signal_table::add(string name, string kind, string type, string belong_function, bool is_global, bool is_array,
                       int array_size, int value, int offset) {
    this->signal_table1.push_back(
            signal_table_element(name, kind, type, belong_function, is_global, is_array, array_size, value, offset));
    this->size = this->size + 1;
}

int signal_table::find_loc(string name,string function_name) {
    for (int i = 0; i < this->size; i++) {
        if (this->get(i).name == name && (this->get(i).belong_function == function_name)) {
            return i;
        }
    }
    for (int i = 0; i < this->size; i++) {
        if (this->get(i).name == name && this->get(i).belong_function == string("#global")) {
            return i;
        }
    }

    //error
    exit(-1);
}

string signal_table::get_kind(string name,string function_name) {
    return get(find_loc(name,function_name)).kind;
}

string signal_table::get_type(string name,string function_name) {
    return get(find_loc(name,function_name)).type;
}

string signal_table::get_belong_function(string name,string function_name) {
    return get(find_loc(name,function_name)).belong_function;
}

bool signal_table::get_is_array(string name,string function_name) {
    return get(find_loc(name,function_name)).is_array;
}

bool signal_table::get_is_global(string name,string now_func) {
    string function_name("#global");
    for (int i = 0; i < this->size; i++) {
        if (this->get(i).name == name && this->get(i).belong_function == now_func) {
            return false;
        }
    }
    for (int i = 0; i < this->size; i++) {
        if (this->get(i).name == name && this->get(i).belong_function == function_name
        || this->get(i).belong_function == string("#global")) {
            return true;
        }
    }
    return false;
}

vector<signal_table_element> signal_table::get_func_var(string now_func){
    vector<signal_table_element> var_vector;
    for (int i = 0; i < this->size; i++) {
        if (this->get(i).belong_function == now_func) {
            var_vector.push_back(this->get(i));
        }
    }
    return var_vector;
}

int signal_table::get_array_size(string name,string function_name) {
    if (get(find_loc(name,function_name)).is_array == false) {
        exit(-1);
        //error
    }
    return get(find_loc(name,function_name)).array_size;
}

int signal_table::get_value(string name,string function_name) {
    return get(find_loc(name,function_name)).value;
}

int signal_table::get_offset(string name,string function_name) {
    return get(find_loc(name,function_name)).offset;
}

void signal_table::change_offset(string name, string func, int offset) {
    this->signal_table1.at(find_loc(name,func)).offset = offset;
}

int signal_table::var_in_function_offset(string func) {
    int count = 0;
    for(int i=0;i<this->signal_table1.size();i++){
        signal_table_element t = this->signal_table1.at(i);
        if(t.belong_function == func && t.kind == string("var")){
            count += 4;
        }
    }
    return count;
}