//
// Created by ym500 on 2019/11/16.
//

#ifndef UNTITLED_FUNCTION_TABLE_H
#define UNTITLED_FUNCTION_TABLE_H

#include <string>
#include <vector>
using namespace std;
class function_table_element{
public:
    string name;
    string type; // void int char
    bool has_return;
    vector<pair<string,string>> param_list;
    function_table_element(string name,string type,bool has_return);
};
class function_table{
private:
    vector<function_table_element> function_table1;
public:
    function_table();
    function_table_element get(int i);
    int get_func_table_size();
    void add(string name,string type,bool has_return);
    void add_param(string function_name,string type,string ident);
    string get_func_type(string name);
    bool get_func_has_return(string name);
    int get_func_param_count(string name);
    int find_loc(string name);
};

void print_all();
#endif //UNTITLED_FUNCTION_TABLE_H
