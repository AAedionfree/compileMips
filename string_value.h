//
// Created by ym500 on 2019/11/17.
//

#ifndef UNTITLED_STRING_VALUE_H
#define UNTITLED_STRING_VALUE_H
#include<string>
using namespace std;

extern string semicolon_str;
extern string comma_str;
extern string const_str;
extern string int_str;
extern string char_str;
extern string void_str;
extern string main_str;
extern string while_str;
extern string return_str;
extern string for_str;
extern string array_r;
extern string array_l;
extern string bracket_l;
extern string bracket_r;
extern string div_str;
extern string mutl_str;
extern string plus_str;
extern string sub_str;
extern string fun_bracket_l;
extern string fun_bracket_r;
extern string assgin_str;
extern string equal_str;
extern string less_str;
extern string less_equal_str;
extern string greater_str;
extern string greater_equal_str;
extern string unequal_str;
extern string if_str;
extern string else_str;
extern string do_str;
extern string printf_str;
extern string scanf_str;
extern string int_const_type;
extern string char_const_type;
extern string signal_type;
extern string string_type;
extern string var_str;
extern string param_str;

//null
extern string null_midcode;
//define and declare
extern string Func_define_midcode;
extern string Param_declare_midcode;
extern string Var_declare_midcode;
extern string Main_define_declare_midcode;
//if
extern string Equal_midcode;
extern string Not_Equal_midcode;
extern string Greater_than_midcode;
extern string Less_than_midcode;
extern string Greater_or_Equal_midcode;
extern string Less_or_Equal_midcode;
//lable
extern string Lable_midcode;
//scanf
extern string Read_int_midcode;
extern string Read_char_midcode;
//printf
extern string Write_string_midcode;
extern string Write_int_midcode;
extern string Write_char_midcode;
//function_end
extern string Function_end_midcode;
//function_return
extern string Function_return_midcode;
// +-*/
extern string VAT_midcode;
extern string ADD_midcode;
extern string SUB_midcode;
extern string MUL_midcode;
extern string DIV_midcode;
//assign
extern string Assign_midcode;
extern string Assign_array_midcode;
//push param
extern string Push_param_midcode;
//save_scence
extern string Save_sence_midcode;
//call function
extern string Call_midcode;
//jump
extern string Jump_midcode;
//enter
extern string Enter_midcode;

//mips
extern string li_mips;
extern string la_mip;
extern string addi_mips;
extern string load_word;
extern string save_word;
#endif //UNTITLED_STRING_VALUE_H
