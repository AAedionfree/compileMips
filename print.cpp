//
// Created by ym500 on 2019/11/16.
//

#include "function_table.h"
#include "siganl_table.h"
#include "midcode.h"
#include "string_value.h"
#include "instruction.h"
#include "control_define.h"
#include <iostream>

using namespace std;

extern vector<instruction> instruction_vector;
extern signal_table table;
extern function_table func_table;
extern vector<midcode> midcode_vector;

void print_all() {
   if(debug){
       cout << endl;
       cout << "符号表" << endl;
       for (int i = 0; i < table.get_size(); i++) {
           cout.width(10);
           cout << table.get(i).name << " ";
           cout.width(10);
           cout << table.get(i).kind << " ";
           cout.width(10);
           cout << table.get(i).type << " ";
           cout.width(10);
           cout << table.get(i).belong_function << " ";
           cout.width(10);
           cout << table.get(i).is_global << " ";
           cout.width(10);
           cout << table.get(i).is_array << " ";
           cout.width(10);
           cout << table.get(i).array_size << " ";
           cout.width(10);
           cout << table.get(i).value << " ";
           cout.width(10);
           cout << table.get(i).offset << " ";
           cout << endl;
       }
       cout << endl;
       cout << "函数表" << endl;
       for (int i = 0; i < func_table.get_func_table_size(); i++) {
           cout.width(15);
           cout << func_table.get(i).name << " ";
           cout.width(10);
           cout << func_table.get(i).type << " ";
           cout.width(10);
           cout << func_table.get(i).has_return << " ";
           cout << endl;
           for (int j = 0; j < func_table.get(i).param_list.size(); j++) {
               cout.width(26);
               cout << func_table.get(i).param_list.at(j).first << " ";
               cout.width(10);
               cout << func_table.get(i).param_list.at(j).second << " ";
               cout << endl;
           }
       }
       cout << endl;
       cout << "中间代码" << endl;
       for (int i = 0; i < midcode_vector.size(); i++) {
           cout.width(10);
           cout << midcode_vector.at(i).get_first_pos() << " ";
           cout.width(10);
           cout << midcode_vector.at(i).get_second_pos() << " ";
           cout.width(10);
           cout << midcode_vector.at(i).get_third_pos() << " ";
           cout.width(10);
           cout << midcode_vector.at(i).get_fourth_pos() << " ";
           cout << endl;
       }
       cout << endl;
       cout << "mips汇编" << endl;
   }
    for (int i = 0; i < instruction_vector.size(); i++) {
        cout.width(15);
        cout << instruction_vector.at(i).instruction_name << " ";
        if(instruction_vector.at(i).first_pos != null_midcode){
            cout.width(15);
            cout << instruction_vector.at(i).first_pos << " ";
        }
        if(instruction_vector.at(i).second_pos != null_midcode){
            cout.width(15);
            cout << instruction_vector.at(i).second_pos << " ";
        }
        if(instruction_vector.at(i).third_pos != null_midcode){
            cout.width(15);
            cout << instruction_vector.at(i).third_pos << " ";
        }
        cout << endl;
    }
}