//
// Created by ym500 on 2019/11/16.
//

#ifndef UNTITLED_SIGANL_TABLE_H
#define UNTITLED_SIGANL_TABLE_H

#include<vector>
#include<string>

using namespace std;

class signal_table_element {
public:
    string name;
    string kind;    //var param const
    string type;    //int/char
    string belong_function;
    bool is_global;
    bool is_array;
    int array_size;
    int value;  //常量
    int offset; //相对于$fp寄存器
    signal_table_element(string name, string kind, string type, string belong_function, bool is_global, bool is_array,
                         int array_size);
    signal_table_element(string name, string kind, string type, string belong_function, bool is_global, bool is_array,
                         int array_size,int value,int offset);
};

class signal_table {
private:
    vector<signal_table_element> signal_table1;
    int size;

public:
    signal_table();

    signal_table_element get(int i);

    void
    add(string name, string kind, string type, string belong_function, bool is_global, bool is_array, int array_size,int value,int offset);

    int get_size();

    int find_loc(string name,string func);

    string get_kind(string name,string func);

    string get_type(string name,string func);

    string get_belong_function(string name,string func);

    bool get_is_array(string name,string func);

    bool get_is_global(string name,string now_func);

    int get_array_size(string name,string func);

    int get_value(string name,string func);

    int get_offset(string name,string func);

    void change_offset(string name,string func,int offset);

    int var_in_function_offset(string func);

    vector<signal_table_element> get_func_var(string now_func);
};

#endif //UNTITLED_SIGANL_TABLE_H
