//
// Created by ym500 on 2019/10/17.
//
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <map>
#include "syntax.h"
#include "split.h"
#include "siganl_table.h"
#include "function_table.h"
#include "midcode.h"
#include "string_value.h"
#include "control_define.h"

using namespace std;
string semicolon_str(";");
string comma_str(",");
string const_str("const");
string int_str("int");
string char_str("char");
string void_str("void");
string main_str("main");
string while_str("while");
string return_str("return");
string for_str("for");
string array_r("]");
string array_l("[");
string bracket_l("(");
string bracket_r(")");
string div_str("/");
string mutl_str("*");
string plus_str("+");
string sub_str("-");
string fun_bracket_l("{");
string fun_bracket_r("}");
string assgin_str("=");
string equal_str("==");
string less_str("<");
string less_equal_str("<=");
string greater_str(">");
string greater_equal_str(">=");
string unequal_str("!=");
string if_str("if");
string else_str("else");
string do_str("do");
string printf_str("printf");
string scanf_str("scanf");
string int_const_type("INTCON");
string char_const_type("CHARCON");
string signal_type("IDENFR");
string string_type("STRCON");
string var_str("var");
string param_str("param");

//null
string null_midcode("#NULL");
//define and declare
string Func_define_midcode("Function_define");
string Param_declare_midcode("Param_declare");
string Var_declare_midcode("Var_declare");
string Main_define_declare_midcode("Main_func_define");
//if
string Equal_midcode("BNE");
string Not_Equal_midcode("BEQ");
string Greater_than_midcode("BLE");
string Less_than_midcode("BGE");
string Greater_or_Equal_midcode("BLT");
string Less_or_Equal_midcode("BGT");
//lable
string Lable_midcode("Lable");
//scanf
string Read_int_midcode("Read_int");
string Read_char_midcode("Read_char");
//printf
string Write_string_midcode("Write_string");
string Write_int_midcode("Write_int");
string Write_char_midcode("Write_char");
//function_end
string Function_end_midcode("Function_end");
//function_return
string Function_return_midcode("Return");
// +-*/
string VAT_midcode("VAT");
string ADD_midcode("ADD");
string SUB_midcode("SUB");
string MUL_midcode("MUL");
string DIV_midcode("DIV");
//assign
string Assign_midcode("Assign");
string Assign_array_midcode("Assign_array");
//push param
string Push_param_midcode("Push");
//save_scence
string Save_sence_midcode("Save_sence");
//call function
string Call_midcode("Call");
//jump
string Jump_midcode("Jump");
//enter
string Enter_midcode("Enter");
vector<pair<string, string> > vector_buffer;

signal_table table = signal_table();
function_table func_table = function_table();
vector<midcode> midcode_vector;

int express_start = 0;
int express_end = 0;

string now_function("#global");
string global_function("#global");
int global = 0;
int vector_buffer_index = 0;

int var_count = 0;
int param_count1 = 0;

string label("_label_condition_end");
int label_number = 0;

string temp_name("@temp");
int temp_name_index = 0;

string return_name("@ret");
int return_name_index = 0;

map<string, string> compareSignalToMidcode = {
        {equal_str,         Equal_midcode},
        {unequal_str,       Not_Equal_midcode},
        {less_str,          Less_than_midcode},
        {less_equal_str,    Less_or_Equal_midcode},
        {greater_str,       Greater_than_midcode},
        {greater_equal_str, Greater_or_Equal_midcode}
};

map<string, string> opToMidcode = {
        {plus_str, ADD_midcode},
        {sub_str,  SUB_midcode},
        {mutl_str, MUL_midcode},
        {div_str,  DIV_midcode}
};

bool is_int_express(int s, int t) {
    pair<string, string> temp = vector_buffer.at(s);
    if (temp.first == char_const_type && s + 1 == t) {
        return false;
    }
    if (temp.first == signal_type && s + 1 == t) {
        if (table.get_type(temp.second, now_function) == char_str) {
            return false;
        }
    }
    if (s + 1 < t) {
        pair<string, string> temp_1 = vector_buffer.at(s + 1);
        if (temp.first == signal_type && temp_1.second == array_l) {
            if(table.get_type(temp.second,now_function) == char_str){
                int count = 0;
                for (int i = s + 1; i < t; i++) {
                    pair<string, string> temp = vector_buffer.at(i);
                    if (temp.second == array_l) {
                        count++;
                    } else if (temp.second == array_r) {
                        count--;
                    }
                    if (count == 0 && i + 1 == t) {
                        return false;
                    } else if (t == 0) {
                        return true;
                    }
                }
            }
        } else if (temp.first == signal_type && temp_1.second == bracket_l) {
            if (func_table.get_func_type(temp.second) == char_str) {
                int count = 0;
                for (int i = s + 1; i < t; i++) {
                    pair<string, string> temp = vector_buffer.at(i);
                    if (temp.second == bracket_l) {
                        count++;
                    } else if (temp.second == bracket_r) {
                        count--;
                    }
                    if (count == 0 && i + 1 == t) {
                        return false;
                    } else if (count == 0) {
                        return true;
                    }
                }
            }
        }
    }
    return true;
}

void error(string s) {
    cout << "error because " << s << " at " << "index " << vector_buffer_index << endl;
    exit(1);
}

pair<string, string> getsym() {
    vector_buffer_index++;
    if (vector_buffer_index > vector_buffer.size()) {
        pair<string, string> print = vector_buffer.at(vector_buffer_index - 2);
        if (print_split) {
            cout << print.first << " " << print.second << endl;
        }
        string eof = string("it's time to return");
        push_back_to_buffer(eof, eof);
    } else if (vector_buffer_index - 2 >= 0) {
        pair<string, string> print = vector_buffer.at(vector_buffer_index - 2);
        if (print_split) {
            cout << print.first << " " << print.second << endl;
        }
    }
    return vector_buffer.at(vector_buffer_index - 1);
}

string sym() {
    return vector_buffer.at(vector_buffer_index - 1).second;
}

string sym_type() {
    return vector_buffer.at(vector_buffer_index - 1).first;
}

string sym_next(int k) {
    if (vector_buffer_index + k - 1 < vector_buffer.size()) {
        return vector_buffer.at(vector_buffer_index - 1 + k).second;
    } else {
        error("cross the vector border");
        return string("");
    }
}

string sym_next_type(int k) {
    if (vector_buffer_index + k - 1 < vector_buffer.size()) {
        return vector_buffer.at(vector_buffer_index - 1 + k).first;
    } else {
        error("cross the vector border");
        return string("");
    }
}

void check(string goal, string error_str, int print = 1) {
    if (sym() == goal) {
        getsym();
        return;
    } else {
        error(error_str);
    }
}

bool check_no_error(string goal, string error_str) {
    if (sym() == goal) {
        getsym();
        return true;
    } else {
        return false;
    }
}

bool check_type(string goal, string error_str, int print = 1) {
    if (sym_type() == goal) {
        getsym();
        return true;
    } else {
        return false;
    }
}


bool check(string goal, string goal2, string error_str) {
    if (sym() == goal) {
        getsym();
        return false;
    } else if (sym() == goal2) {
        getsym();
        return true;
    } else {
        error(error_str);
        //can not reach
        return false;
    }
}

bool in_has_return_form(string str) {
    return func_table.get_func_has_return(str);
}

bool in_no_return_form(string str) {
    return !func_table.get_func_has_return(str);
}

void process() {
    global = 1;
    if (sym() == const_str) {
        const_declare();
    }
    var_count = 0;
    if ((sym() == int_str || sym() == char_str) && (sym_next(2) != bracket_l)) {
        var_declare();
    }

    add_MidcodeToVector(Var_declare_midcode, null_midcode, null_midcode, to_string(var_count));

    global = 0;
    while (sym() != void_str || sym_next(1) != main_str) {
        if (sym() == int_str || sym() == char_str) {
            has_return_func();
        } else if (sym() == void_str) {
            no_return_func();
        } else {
            error("function signal");
        }
    }
    main_check();
    if (print_syntax) {
        cout << "<程序>" << endl;
    }
}

int check_no_signal_int() {
    int number = stoi(sym());
    check_type(int_const_type, "lose int const in (check_int)");
    if (print_syntax) {
        cout << "<无符号整数>" << endl;
    }
    return number;
}

int check_int() {
    string t = sym();
    if (sym() == plus_str || sym() == sub_str) {
        check(plus_str, sub_str, "lose + or - in (check_int)");
    }
    int number = check_no_signal_int();
    if (t == sub_str) {
        number = -number;
    }
    if (print_syntax) {
        cout << "<整数>" << endl;
    }
    return number;
}

void const_declare() {
    while (sym() == const_str) {
        getsym();
        const_define();
        check(semicolon_str, "loss semicolon(const_declare)");
    }
    if (print_syntax) {
        cout << "<常量说明>" << endl;
    }
}

void const_define() {
    int flag = 0;
    bool is_char;
    do {
        if (flag != 0) {
            getsym();
        } else {
            flag = 1;
            is_char = check(int_str, char_str, "const define not int or char(const_define)");
        }
        string name = sym();
        check_type(signal_type, "lose signal string(const_define)");
        check(assgin_str, "lose assigin string(const_define)");
        if (is_char) {
            string char_const = sym();
            table.add(name, const_str, char_str, now_function, global, 0, 1, char_const[0], -1);
            check_type(char_const_type, "lose char const(const_define)");
        } else {
            int v = check_int();
            table.add(name, const_str, int_str, now_function, global, 0, 1, v, -1);
        }
    } while (sym() == comma_str);
    if (print_syntax) {
        cout << "<常量定义>" << endl;
    }
}

void var_declare() {
    while ((sym() == int_str || sym() == char_str) && sym_next(2) != bracket_l) {
        string type = sym();
        getsym();
        var_define(type);
        check(semicolon_str, "lose semicolon(var_declare)");
    }
    if (print_syntax) {
        cout << "<变量说明>" << endl;
    }
}

void var_define(string type) {
    int flag = 0;
    do {
        if (flag != 0) {
            check(comma_str, "lose comma");
        } else {
            flag = 1;
        }
        string name = sym();
        check_type(signal_type, "lose signal string(var_define)");
        if (check_no_error(array_l, "lose array left [ (var_define)")) {
            int size = check_no_signal_int();
            table.add(name, var_str, type, now_function, global, true, size, 0, var_count);
            var_count += 4 * size;
            check(array_r, "lose array right ] (var define)");
        } else {
            table.add(name, var_str, type, now_function, global, false, 0, 0, var_count);
            var_count += 4;
        }
    } while (sym() == comma_str);
    if (print_syntax) {
        cout << "<变量定义>" << endl;
    }
}

void head_of_declaration() {
    string type = sym();
    check(int_str, char_str, "lose head of declaration(head_of_declaration)");
    string name = sym();
    now_function = sym();

    add_MidcodeToVector(Func_define_midcode, name, type, null_midcode);
    func_table.add(name, type, true);

    check_type(signal_type, "lose signal(head_of_declaration)");
    if (print_syntax) {
        cout << "<声明头部>" << endl;
    }
}

void param_list() {
    int flag = 0;
    param_count1 = 0;
    if (sym() != ")") {
        do {
            if (flag != 0) {
                getsym();
            } else {
                flag = 1;
            }
            string type = sym();
            check(int_str, char_str, "lose int or char type define(param_list)");
            string name = sym();
            check_type(signal_type, "lose signal(param_list)");

            add_MidcodeToVector(Param_declare_midcode, name, type, null_midcode);
            table.add(name, param_str, type, now_function, false, false, 0, 0, -1);
            param_count1 += 4;
            func_table.add_param(now_function, type, name);
        } while (sym() == comma_str);
    }
    if (print_syntax) {
        cout << "<参数表>" << endl;
    }
}

void has_return_func() {
    var_count = 0;
    head_of_declaration();
    check(bracket_l, "lose bracket_l ( (has_return_func)");
    param_list();
    check(bracket_r, "lose bracket_r ) (has_return_func)");
    check(fun_bracket_l, "lose bracket_l { (has_return_func)");
    complex_sentence();
    check(fun_bracket_r, "lose bracket_r } (has_return_func)");
    add_MidcodeToVector(Function_end_midcode, now_function, null_midcode, null_midcode);
    now_function = global_function;
    if (print_syntax) {
        cout << "<有返回值函数定义>" << endl;
    }
}

void no_return_func() {
    var_count = 0;
    string type = sym();
    check(void_str, "lose void keywords(no_return_func)");
    string name = sym();
    now_function = sym();

    add_MidcodeToVector(Func_define_midcode, name, type, null_midcode);
    func_table.add(name, type, false);

    check_type(signal_type, "lose signal(no_return_func)");
    check(bracket_l, "lose bracket_l ( (no_return_func)");
    param_list();
    check(bracket_r, "lose bracket_r ) (no_return_func)");
    check(fun_bracket_l, "lose bracket_l { (no_return_func)");
    complex_sentence();
    check(fun_bracket_r, "lose bracket_r } (no_return_func)");
    add_MidcodeToVector(Function_end_midcode, now_function, null_midcode, null_midcode);
    now_function = global_function;
    if (print_syntax) {
        cout << "<无返回值函数定义>" << endl;
    }
}

void complex_sentence() {
    if (sym() == const_str) {
        const_declare();
    }
//    var_count = 0;
    if ((sym() == int_str || sym() == char_str) && (sym_next(2) != bracket_l)) {
        var_declare();
    }

    add_MidcodeToVector(Var_declare_midcode, null_midcode, null_midcode, to_string(var_count + param_count1));

    sentence_list();
    if (print_syntax) {
        cout << "<复合语句>" << endl;
    }
}

void sentence_list() {
    while (sym() == if_str                                                      // if
           || sym() == while_str || sym() == do_str || sym() == for_str         // loop
           || ((sym_type() == signal_type) && (sym_next(1) == bracket_l))    // invoke function
           || ((sym_type() == signal_type) && (sym_next(1) == assgin_str))   // assign sentence
           || ((sym_type() == signal_type) && (sym_next(1) == array_l))
           || sym() == printf_str                                               // printf
           || sym() == scanf_str                                                // scanf
           || sym() == return_str                                               // return
           || sym() == semicolon_str                                            // ;
           || sym() == fun_bracket_l
            ) {
        sentence();
    }
    if (print_syntax) {
        cout << "<语句列>" << endl;
    }
}

void sentence() {
    if (sym() == if_str) {
        if_sentence();
    } else if (sym() == while_str || sym() == do_str || sym() == for_str) {
        loop_sentence();
    } else if ((sym_type() == signal_type) && (sym_next(1) == bracket_l)) {
        if (in_has_return_form(sym())) {
            invoke_has_return_func();
        } else if (in_no_return_form(sym())) {
            invoke_no_return_func();
        } else {
            error("not found function in function form(sentence)");
        }
        check(semicolon_str, "lose semicolon in (sentence)");
    } else if ((sym_type() == signal_type) && ((sym_next(1) == assgin_str) || (sym_next(1) == array_l))) {
        assgin_sentence();
        check(semicolon_str, "lose semicolon in (sentence)");
    } else if (sym() == printf_str) {
        write_sentence();
        check(semicolon_str, "lose semicolon in (sentence)");
    } else if (sym() == scanf_str) {
        read_sentence();
        check(semicolon_str, "lose semicolon in (sentence)");
    } else if (sym() == return_str) {
        return_sentence();
        check(semicolon_str, "lose semicolon in (sentence)");
    } else if (sym() == semicolon_str) {
        getsym();
    } else if (sym() == fun_bracket_l) {
        check(fun_bracket_l, "lose { in (sentence)");
        sentence_list();
        check(fun_bracket_r, "lose } in (sentence)");
    } else {
        error("sentence not match(sentence)");
    }
    if (print_syntax) {
        cout << "<语句>" << endl;
    }
}

void if_sentence() {
    check(if_str, "lose if keywords(if_sentence)");
    check(bracket_l, "lose ( (if_sentence");
    int condition_number = label_number;
    condition();
    check(bracket_r, "lose ) (if_sentence)");
    sentence();
    int condition_number1 = label_number;
    add_MidcodeToVector(Jump_midcode, null_midcode, label + to_string(label_number++), null_midcode);
    add_MidcodeToVector(Lable_midcode, label + to_string(condition_number), null_midcode, null_midcode);
    if (sym() == else_str) {
        getsym();
        sentence();
    }
    add_MidcodeToVector(Lable_midcode, label + to_string(condition_number1), null_midcode, null_midcode);
    if (print_syntax) {
        cout << "<条件语句>" << endl;
    }
}

void loop_sentence() {
    if (sym() == while_str) {
        check(while_str, "lose while keywords(loop_sentence)");
        check(bracket_l, "lose ( (loop_sentence)");
        int back_number = label_number;
        add_MidcodeToVector(Lable_midcode, label + to_string(label_number), null_midcode, null_midcode);
        label_number++;
        int condition_number = label_number;
        condition();

        check(bracket_r, "lose ) (loop_sentence)");
        sentence();
        add_MidcodeToVector(Jump_midcode, null_midcode, label + to_string(back_number), null_midcode);
        add_MidcodeToVector(Lable_midcode, label + to_string(condition_number), null_midcode, null_midcode);
    } else if (sym() == do_str) {
        string start = label + to_string(label_number++);
        add_MidcodeToVector(Lable_midcode, start, null_midcode, null_midcode);
        check(do_str, "lose const string do in (loop_sentence)");
        sentence();
        check(while_str, "lose const string while(loop_sentence)");
        check(bracket_l, "lose ( in (loop_sentence)");

        int condition_number = label_number;
        condition();

        check(bracket_r, "lose ) in (loop_sentence)");
        add_MidcodeToVector(Jump_midcode, null_midcode, start, null_midcode);
        add_MidcodeToVector(Lable_midcode, label + to_string(condition_number), null_midcode, null_midcode);
    } else if (sym() == for_str) {
        check(for_str, "lose const string for in(loop_sentence)");
        check(bracket_l, "lose ( in (loop_sentence)");

        string name = sym();
        check_type(signal_type, "lose signal_define in (loop_sentence)");
        check(assgin_str, "lose = in (loop_sentence)");
        value_midcode exp_value = express();
        //init
        add_MidcodeToVector(Assign_midcode, name, null_midcode, exp_value.temp_name);
        //start label
        string start = label + to_string(label_number++);
        add_MidcodeToVector(Lable_midcode, start, null_midcode, null_midcode);
        check(semicolon_str, "lose semicolon (loop_sentence)");

        int condition_number = label_number;
        // condition jump
        condition();
        check(semicolon_str, "lose semicolon (loop_sentence)");

        string ident1 = sym();
        check_type(signal_type, "lose signal_define in (loop_sentence)");
        check(assgin_str, "lose = in (loop_sentence)");
        string ident2 = sym();
        check_type(signal_type, "lose signal_define in (loop_sentence)");
        string op = sym();
        check(plus_str, sub_str, "lose + or - in (loop_sentence)");
        int stp = step();

        check(bracket_r, "lose ) in (loop_sentence)");
        string loop_pos;

        sentence();

        if (op == plus_str) {
            add_MidcodeToVector(ADD_midcode, temp_name + to_string(temp_name_index), ident2, to_string(stp));
        } else if (op == sub_str) {
            add_MidcodeToVector(SUB_midcode, temp_name + to_string(temp_name_index), ident2, to_string(stp));
        }
        add_MidcodeToVector(Assign_midcode, ident1, null_midcode, temp_name + to_string(temp_name_index));
        temp_name_index++;
        add_MidcodeToVector(Jump_midcode, null_midcode, start, null_midcode);
        add_MidcodeToVector(Lable_midcode, label + to_string(condition_number), null_midcode, null_midcode);
    } else {
        error("can not match while or do or for in(loop_sentence)");
    }
    if (print_syntax) {
        cout << "<循环语句>" << endl;
    }
}

void condition() {
    value_midcode exp_1 = express();
    string op = sym();
    if (sym() == less_str || sym() == less_equal_str
        || sym() == greater_str || sym() == greater_equal_str
        || sym() == unequal_str || sym() == equal_str
            ) {
        //exp exp
        getsym();
        value_midcode exp_2 = express();
        add_MidcodeToVector(compareSignalToMidcode[op], label + to_string(label_number++), exp_1.temp_name,
                            exp_2.temp_name);
    } else {
        add_MidcodeToVector(compareSignalToMidcode[unequal_str], label + to_string(label_number++), exp_1.temp_name,
                            string("0"));
    };
    if (print_syntax) {
        cout << "<条件>" << endl;
    }
}

value_midcode::value_midcode(bool is_const, int const_value, string temp_name) {
    this->is_const = is_const;
    this->const_value = const_value;
    this->temp_name = temp_name;
}

value_midcode express() {
    int t_start = vector_buffer_index - 1;
    string first_op("#");
    if ((sym() == plus_str || sym() == sub_str) && sym_type() != char_const_type && sym_next_type(1) == signal_type) {
        first_op = sym();
        getsym();
    }
    int flag = 0;
    value_midcode before_value;
    do {
        string op;
        if (flag != 0) {
            op = sym();
            check(plus_str, sub_str, "lose + or - (express)");
        }
        value_midcode now_value = item();
        if (flag == 0) {
            before_value = now_value;
            if (first_op == sub_str) {
                if (before_value.is_const) {
                    before_value.const_value = -before_value.const_value;
                    before_value.temp_name = string("-") + before_value.temp_name;
                } else {
                    add_MidcodeToVector(MUL_midcode, temp_name + to_string(temp_name_index), string("-1"),
                                        before_value.temp_name);
                    before_value = value_midcode(false, 0, temp_name + to_string(temp_name_index));
                    temp_name_index++;
                }
            }
            flag = 1;
        } else {
            if (before_value.is_const && now_value.is_const) {
                int t = 0;
                if (op == plus_str) {
                    t = before_value.const_value + now_value.const_value;
                } else if (op == sub_str) {
                    t = before_value.const_value - now_value.const_value;
                } else {
                    error("op error in (express)");
                }
                before_value = value_midcode(true, t, to_string(t));
            } else {
                add_MidcodeToVector(opToMidcode[op], temp_name + to_string(temp_name_index),
                                    before_value.temp_name, now_value.temp_name);
                before_value = value_midcode(false, 0, temp_name + to_string(temp_name_index));
                temp_name_index++;
            }
        }
    } while (sym() == plus_str || sym() == sub_str);
    express_start = t_start;
    express_end = vector_buffer_index - 1;
    if (print_syntax) {
        cout << "<表达式>" << endl;
    }
    return before_value;
}

value_midcode item() {
    int flag = 0;
    //ident first item
    value_midcode before_value;
    do {
        string op;
        if (flag != 0) {
            op = sym();
            check(mutl_str, div_str, "lose mutl or div * / (item)");
        }
        value_midcode now_value = factor();
        if (flag == 0) {
            before_value = now_value;
            flag = 1;
        } else {
            if (before_value.is_const && now_value.is_const) {
                int t = 0;
                if (op == mutl_str) {
                    t = before_value.const_value * now_value.const_value;
                } else if (op == div_str) {
                    t = before_value.const_value / now_value.const_value;
                } else {
                    error("op error in (item)");
                }
                before_value = value_midcode(true, t, to_string(t));
            } else {
                add_MidcodeToVector(opToMidcode[op], temp_name + to_string(temp_name_index),
                                    before_value.temp_name, now_value.temp_name);
                before_value = value_midcode(false, 0, temp_name + to_string(temp_name_index));
                temp_name_index++;
            }
        }
    } while (sym() == mutl_str || sym() == div_str);
    if (print_syntax) {
        cout << "<项>" << endl;
    }
    return before_value;
}

value_midcode factor() {
    value_midcode v(false, 0, string("###"));
    //signal
    //signal[]
    //func()
    if (sym_type() == signal_type) {
        if (sym_next(1) == array_l) {
            //array
            string name = sym();
            check_type(signal_type, "lose signal define(factor)");
            check(array_l, "lose array_l [ define(factor)");

            value_midcode exp_index = express();

            check(array_r, "lose array_r ] define(factor)");

            add_MidcodeToVector(VAT_midcode, temp_name + to_string(temp_name_index), name, exp_index.temp_name);
            v = value_midcode(false, 0, temp_name + to_string(temp_name_index));
            temp_name_index++;
        } else if (sym_next(1) == bracket_l) {
            if (!in_has_return_form(sym())) {
                error("don't has return value");
            }
            v = invoke_has_return_func();
        } else if (sym_type() == signal_type) {
            string name = sym();
            if (table.get_kind(name, now_function) == const_str) {
                v = value_midcode(true, table.get_value(name, now_function),
                                  to_string(table.get_value(name, now_function)));
            } else {
                v = value_midcode(false, 0, name);
            }
            check_type(signal_type, "lose signal define(item)");
        } else {
            error("match error in (item)");
        }
    } else if (sym_type() == int_const_type
               || ((sym() == plus_str || sym() == sub_str) && (sym_next_type(1) == int_const_type))) {
        int t = check_int();
        v = value_midcode(true, t, to_string(t));
    } else if (sym_type() == char_const_type) {
        string t = sym();
        check_type(char_const_type, "lose char const (factor)");
        v = value_midcode(true, t[0], to_string(t[0]));
    } else if (sym() == bracket_l) {
        check(bracket_l, "lose a ( (factor)");
        v = express();
        check(bracket_r, "lose a ) (factor)");
    } else {
        error("no match in (factor)");
    }
    if (print_syntax) {
        cout << "<因子>" << endl;
    }
    return v;
}

value_midcode invoke_has_return_func() {
    string name = sym();
    add_MidcodeToVector(Save_sence_midcode, null_midcode, now_function, name);

    check_type(signal_type, "lose signal define (invoke_has_return_func)");
    check(bracket_l, "lose func ( (invoke_has_return_func)");
    value_param_list(name);
    check(bracket_r, "lose func ) (invoke_has_return_func)");

    if (print_syntax) {
        cout << "<有返回值函数调用语句>" << endl;
    }

    add_MidcodeToVector(Call_midcode, return_name + to_string(return_name_index), name, null_midcode);
    value_midcode return_value = value_midcode(false, 0, return_name + to_string(return_name_index));
    return_name_index++;
    return return_value;
}

void invoke_no_return_func() {
    string name = sym();
    add_MidcodeToVector(Save_sence_midcode, null_midcode, now_function, name);
    check_type(signal_type, "lose signal define (invoke_no_return_func)");
    check(bracket_l, "lose func ( (invoke_no_return_func)");
    value_param_list(name);
    check(bracket_r, "lose func ) (invoke_no_return_func)");
    add_MidcodeToVector(Call_midcode, null_midcode, name, null_midcode);
    if (print_syntax) {
        cout << "<无返回值函数调用语句>" << endl;
    }
}

void value_param_list(string func_name) {
    int flag = 0;
    int count = 0; //offset
    if (sym() != bracket_r) {
        do {
            if (flag != 0) {
                check(comma_str, "lose comma (value_param_list)");
            } else {
                flag = 1;
            }
            value_midcode exp_param = express();
            add_MidcodeToVector(Push_param_midcode, func_name, to_string(count), exp_param.temp_name);
            count += 4;
        } while (sym() == comma_str);
    }
    if (print_syntax) {
        cout << "<值参数表>" << endl;
    }
}

void assgin_sentence() {
    string name = sym();
    check_type(signal_type, "lose signal type (assign sentence)");
    if (sym() == assgin_str) {
        check(assgin_str, "lose assign = (assign_sentence)");
        value_midcode exp_value = express();
        add_MidcodeToVector(Assign_midcode, name, null_midcode, exp_value.temp_name);
    } else if (sym() == array_l) {
        check(array_l, "lose array_l [ (assign_sentence)");

        value_midcode exp_index = express();
        check(array_r, "lose array_r ] (assign_sentence)");
        check(assgin_str, "lose assign = (assign_sentence)");
        value_midcode exp_value = express();
        add_MidcodeToVector(Assign_array_midcode, name, exp_index.temp_name, exp_value.temp_name);
    } else {
        error("can't match = or [ in (assign_sentence)");
    }
    if (print_syntax) {
        cout << "<赋值语句>" << endl;
    }
}

void read_sentence() {
    check(scanf_str, "lose const string scanf (read_sentence)");
    check(bracket_l, "lose ( in (read_sentence)");
    int flag = 0;
    do {
        if (flag != 0) {
            check(comma_str, "lose comma , (read_sentence)");
        } else {
            flag = 1;
        }
        string name = sym();
        check_type(signal_type, "lose signal define (read_sentence)");
        string type = table.get_type(name, now_function);
        if (type == int_str) {
            add_MidcodeToVector(Read_int_midcode, name, null_midcode, null_midcode);
        } else {
            add_MidcodeToVector(Read_char_midcode, name, null_midcode, null_midcode);
        };
    } while (sym() == comma_str);
    check(bracket_r, "lose ) in (read_sentence)");
    if (print_syntax) {
        cout << "<读语句>" << endl;
    }
}

string deal(string str1) {
    char ch[1];
    ch[0] = '\\';
    for (int i = 0; i < str1.length(); i++) {
        if (str1[i] == '\\') {
            str1.insert(i, string("\\"));
            i++;
        }
    }
    return str1;
}

void write_sentence() {
    check(printf_str, "lose const string printf (write_sentence)");
    check(bracket_l, "lose ( in (write_sentence)");
    if (sym_type() == string_type) {
        string str = sym();
        check_type(string_type, "lose string in (write_sentence)");
        if (print_syntax) {
            cout << "<字符串>" << endl;
        }
        add_MidcodeToVector(Write_string_midcode, deal(str), null_midcode, null_midcode);
        if (sym() == comma_str) {
            getsym();
            value_midcode exp_value = express();
            if (is_int_express(express_start, express_end)) {
                add_MidcodeToVector(Write_int_midcode, exp_value.temp_name, null_midcode, null_midcode);
            } else {
                add_MidcodeToVector(Write_char_midcode, exp_value.temp_name, null_midcode, null_midcode);
            }
        }
    } else {
        value_midcode exp_value = express();
        if (is_int_express(express_start, express_end)) {
            add_MidcodeToVector(Write_int_midcode, exp_value.temp_name, null_midcode, null_midcode);
        } else {
            add_MidcodeToVector(Write_char_midcode, exp_value.temp_name, null_midcode, null_midcode);
        }
    }
    add_MidcodeToVector(Enter_midcode, null_midcode, null_midcode, null_midcode);
    check(bracket_r, "lose ) in (write_sentence)");
    if (print_syntax) {
        cout << "<写语句>" << endl;
    }
}

int step() {
    int result = check_no_signal_int();
    if (print_syntax) {
        cout << "<步长>" << endl;
    }
    return result;
}

void return_sentence() {
    value_midcode v;
    check(return_str, "can not match string const return (return_sentence)");
    if (sym() == bracket_l) {
        check(bracket_l, "lose ( in (return_sentence)");
        v = express();
        check(bracket_r, "lose ) in (return_sentence)");
    }
    if (print_syntax) {
        cout << "<返回语句>" << endl;
    }
    add_MidcodeToVector(Function_return_midcode, v.temp_name, now_function, null_midcode);
}

void main_check() {
    var_count = 0;
    add_MidcodeToVector(Main_define_declare_midcode, main_str, void_str, null_midcode);
    func_table.add(main_str, void_str, false);

    now_function = main_str;
    check(void_str, "lose string const void (main_check)");
    check(main_str, "lose string const main (main_check)");
    check(bracket_l, "lose ( in (main_check)");
    check(bracket_r, "lose ) in (main_check)");
    check(fun_bracket_l, "lose { in (main_check)");
    complex_sentence();
    check(fun_bracket_r, "lose } in (main_check)");
    add_MidcodeToVector(Function_end_midcode, now_function, null_midcode, null_midcode);
    if (print_syntax) {
        cout << "<主函数>" << endl;
    }
}