//
// Created by ym500 on 2019/11/19.
//

#ifndef UNTITLED_REGISTER_H
#define UNTITLED_REGISTER_H
#include <string>
using namespace std;
class Register{
private:
    string name;
    string value_name;
    string func_name;
    int value;
    int save_var_table_pos;
    int use_time;
    bool is_used;
public:
    Register(string,int,int,int,bool);
    string get_name();
    string get_func_name();
    int get_value();
    int get_save_var_index();
    int get_use_time();
    string get_value_name();
    bool get_is_used();
    void use_time_plus();
    void set_is_unused();
    void set_register(string value_name,string func_name);
    void set_register(string value_name);
};
int get_register_index(string register_name);
//void have_one_clock();
#endif //UNTITLED_REGISTER_H
