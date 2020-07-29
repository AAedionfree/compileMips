//
// Created by ym500 on 2019/11/19.
//

#include <iostream>
#include <map>
#include "Register.h"
#include "instruction.h"
#include "midcode.h"
#include "siganl_table.h"
#include "function_table.h"
#include "string_value.h"
#include "control_define.h"
#include <vector>

using namespace std;

vector<string> string_buffer;
vector<Register> GRF;
extern signal_table table;
extern vector<midcode> midcode_vector;
extern function_table func_table;
//
//vector<pair<string, int>> function_param_count;

//mips code
string li_mips("li");
string la_mips("la");
string add_mips("add");
//string sub_mips("sub");
string addi_mips("addi");
string subi_mips("subi");
string mul_mips("mul");
string div_mips("div");
string load_word_mips("lw");
string save_word_mips("sw");
string j_mips("j");
string jr_mips("jr");
string jal_mips("jal");
string move_mips("move");
//mips reg
string global_var_base_address_reg("$t9");
string var_base_address_reg("$v1");
string stack_reg("$sp");
string reg_v0("$v0");
string reg_a0("$a0");
string zero_reg("$0");
string reg_temp_1("$t1");
string reg_temp_2("$t2");

string save_return_value_reg("$fp");
string rd_reg("$a3");
string rs_reg("$k0");
string rt_reg("$k1");

string ra_reg("$ra");

string reg_temp_3("$t3");
//other
string zero_str("0");
//signal
string now_func("#global");

vector<int> register_s_vector;

vector<int> register_t_vector;

vector<vector<Register>> function_reg_buffer;
int function_reg_buffer_index = 0;

int param_count = 0;
string var_count;
map<string, string> midcodeToMipcode = {
        {ADD_midcode, addi_mips},
        {SUB_midcode, subi_mips},
        {MUL_midcode, mul_mips},
        {DIV_midcode, div_mips}
};

void create_GRF() {
    GRF.push_back(Register(string("$0"), 0, 0, 0, false));
    GRF.push_back(Register(string("$at"), 0, 0, 0, false));
    GRF.push_back(Register(string("$v0"), 0, 0, 0, false));
    GRF.push_back(Register(string("$v1"), 0, 0, 0, false));
    GRF.push_back(Register(string("$a0"), 0, 0, 0, false));
    GRF.push_back(Register(string("$a1"), 0, 0, 0, false));

    GRF.push_back(Register(string("$a2"), 0, 0, 0, false));
    GRF.push_back(Register(string("$a3"), 0, 0, 0, false));

    GRF.push_back(Register(string("$t0"), 0, 0, 0, false));
    GRF.push_back(Register(string("$t1"), 0, 0, 0, false));
    GRF.push_back(Register(string("$t2"), 0, 0, 0, false));
    GRF.push_back(Register(string("$t3"), 0, 0, 0, false));
    GRF.push_back(Register(string("$t4"), 0, 0, 0, false));
    GRF.push_back(Register(string("$t5"), 0, 0, 0, false));
    GRF.push_back(Register(string("$t6"), 0, 0, 0, false));
    GRF.push_back(Register(string("$t7"), 0, 0, 0, false));
    GRF.push_back(Register(string("$s0"), 0, 0, 0, false));
    GRF.push_back(Register(string("$s1"), 0, 0, 0, false));
    GRF.push_back(Register(string("$s2"), 0, 0, 0, false));
    GRF.push_back(Register(string("$s3"), 0, 0, 0, false));
    GRF.push_back(Register(string("$s4"), 0, 0, 0, false));
    GRF.push_back(Register(string("$s5"), 0, 0, 0, false));
    GRF.push_back(Register(string("$s6"), 0, 0, 0, false));
    GRF.push_back(Register(string("$s7"), 0, 0, 0, false));
    GRF.push_back(Register(string("$t8"), 0, 0, 0, false));
    GRF.push_back(Register(string("$t9"), 0, 0, 0, false));
    GRF.push_back(Register(string("$k0"), 0, 0, 0, false));
    GRF.push_back(Register(string("$k1"), 0, 0, 0, false));
    GRF.push_back(Register(string("$gp"), 0, 0, 0, false));
    GRF.push_back(Register(string("$sp"), 0, 0, 0, false));
    GRF.push_back(Register(string("$fp"), 0, 0, 0, false));
    GRF.push_back(Register(string("$ra"), 0, 0, 0, false));
    register_s_vector.push_back(16);
    register_s_vector.push_back(17);
    register_s_vector.push_back(18);
    register_s_vector.push_back(19);
    register_s_vector.push_back(20);
    register_s_vector.push_back(21);
    register_s_vector.push_back(22);
    register_s_vector.push_back(23);

    register_t_vector.push_back(8);
    register_t_vector.push_back(9);
    register_t_vector.push_back(10);
    register_t_vector.push_back(11);
    register_t_vector.push_back(12);
    register_t_vector.push_back(13);
    register_t_vector.push_back(14);
    register_t_vector.push_back(15);
    register_t_vector.push_back(24);
    register_t_vector.push_back(25);
}

bool is_CONST(string name) {
    return ((name[0] >= '0' && name[0] <= '9') || name[0] == '+' || name[0] == '-');
}

bool is_ident(string name) {
    return !((name[0] >= '0' && name[0] <= '9') || name[0] == '+' || name[0] == '-') && name[0] != '@';
}

bool is_temp_midcode(string name) {
    return name[0] == '@';
}


void cout_note(string note) {
    if (debug) {
        cout << "#" << note << endl;
        push_to_instruction_vector(string("#") + note);
    }
}

void print_midcode(midcode t) {
    cout_note(t.get_first_pos() + " " + t.get_second_pos() + " " + t.get_third_pos() + " " + t.get_fourth_pos());
}

bool register_s_vector_full() {
    for (int i = 0; i < register_s_vector.size(); i++) {
        if (!GRF.at(register_s_vector.at(i)).get_is_used()) {
            return false;
        }
    }
    return true;
}

string alloc_s_Reg2Var(string var_name, string func_name) {
    for (int i = 0; i < register_s_vector.size(); i++) {
        if (!GRF.at(register_s_vector.at(i)).get_is_used()) {
            GRF.at(register_s_vector.at(i)).set_register(var_name, func_name);
            cout_note("alloc " + GRF.at(register_s_vector.at(i)).get_name() + " to " + var_name);
            return GRF.at(register_s_vector.at(i)).get_name();
        }
    }
}

bool var_in_Svector(string var_name, string now_func) {
    for (int i = 0; i < register_s_vector.size(); i++) {
        if (GRF.at(register_s_vector.at(i)).get_is_used() &&
            GRF.at(register_s_vector.at(i)).get_value_name() == var_name
            && (GRF.at(register_s_vector.at(i)).get_func_name() == now_func ||
                GRF.at(register_s_vector.at(i)).get_func_name() == string("#global"))) {
            return true;
        }
    }
    return false;
}

string find_var_from_Svector(string var_name, string now_func) {
    for (int i = 0; i < register_s_vector.size(); i++) {
        if (GRF.at(register_s_vector.at(i)).get_is_used() &&
            GRF.at(register_s_vector.at(i)).get_value_name() == var_name
            && GRF.at(register_s_vector.at(i)).get_func_name() == now_func) {
            return GRF.at(register_s_vector.at(i)).get_name();
        }
    }
    for (int i = 0; i < register_s_vector.size(); i++) {
        if (GRF.at(register_s_vector.at(i)).get_is_used() &&
            GRF.at(register_s_vector.at(i)).get_value_name() == var_name
            && GRF.at(register_s_vector.at(i)).get_func_name() == string("#global")) {
            return GRF.at(register_s_vector.at(i)).get_name();
        }
    }
    return null_midcode;
}

string alloc_register(string var_name, vector<int> list) {
    int max_time = -999999;
    int index = -1;
    for (int i = 0; i < list.size(); i++) {
        if (GRF.at(list.at(i)).get_use_time() > max_time) {
            max_time = GRF.at(list.at(i)).get_use_time();
            index = i;
        }
        if (!GRF.at(list.at(i)).get_is_used()) {
            GRF.at(list.at(i)).set_register(var_name);
            cout_note("alloc " + GRF.at(list.at(i)).get_name() + " to " + var_name);
            return GRF.at(list.at(i)).get_name();
        }
    }
    GRF.at(index).set_register(var_name);
    cout_note("alloc " + GRF.at(index).get_name() + " to " + var_name);
    return GRF.at(index).get_name();
}

string get_register_name_by_VarName(string var_name) {
    for (int i = 0; i < GRF.size(); i++) {
        if (GRF.at(i).get_value_name() == var_name) {
            GRF.at(i).set_is_unused();
            return GRF.at(i).get_name();
        }
    }
}

void load_var_address2Register(string var_name, string register_name, string now_function) {
    bool is_global = table.get_is_global(var_name, now_function);
    int offset = table.get_offset(var_name, now_function);
    if (is_global) {
        push_to_instruction_vector(addi_mips, register_name, global_var_base_address_reg, to_string(offset));
    } else {
        push_to_instruction_vector(addi_mips, register_name, var_base_address_reg, to_string(offset));
    }
}

void load_var_to_Register(string var_name, string register_name, string now_function) {
    int offset = table.get_offset(var_name, now_function);
    //lw
    push_to_instruction_vector(load_word_mips, register_name, to_string(offset),
                               bracket_l + var_base_address_reg + bracket_r);
}

void load_global_var_to_Register(string var_name, string register_name, string now_function) {
    int offset = table.get_offset(var_name, now_function);
    //lw
    push_to_instruction_vector(load_word_mips, register_name, to_string(offset),
                               bracket_l + global_var_base_address_reg + bracket_r);
}

void load_Var2Register(string var_name, string register_name, string now_function) {
    bool is_global = table.get_is_global(var_name, now_function);
    if (is_global) {
        load_global_var_to_Register(var_name, register_name, now_function);
    } else {
        load_var_to_Register(var_name, register_name, now_function);
    }
}

void save_Register_to_var(string var_name, string register_name, string now_function) {
    int offset = table.get_offset(var_name, now_function);
    // sw
    push_to_instruction_vector(save_word_mips, register_name, to_string(offset),
                               bracket_l + var_base_address_reg + bracket_r);
}

void save_Register_to_global_var(string var_name, string register_name, string now_function) {
    int offset = table.get_offset(var_name, now_function);
    // sw
    push_to_instruction_vector(save_word_mips, register_name, to_string(offset),
                               bracket_l + global_var_base_address_reg + bracket_r);
}

void save_Register2Var(string var_name, string register_name, string now_function) {
    bool is_global = table.get_is_global(var_name, now_function);
    if (is_global) {
        save_Register_to_global_var(var_name, register_name, now_function);
    } else {
        save_Register_to_var(var_name, register_name, now_function);
    }
}

void syscall() {
    push_to_instruction_vector(string("syscall"));
}

void load_const_to_Register(string s_const, string register_name) {
    //li
    push_to_instruction_vector(li_mips, register_name, s_const);
}

int deal_size(string str1) {
    int size = 0;
    for (int i = 0; i < str1.length(); i++) {
        if (str1[i] == '\\') {
            if (i + 1 < str1.length()) {
                if (str1[i] == '\\' && str1[i + 1] == '\\') {
                    size += 1;
                    i++;
                }
            }
        } else {
            size += 1;
        }
    }
    return size;
}

void distribute_memory_for_string() {
    string name("str");
    int sting_name_index = 0;
    int size = 0;
    push_to_instruction_vector(string(".data"));
    for (int i = 0; i < midcode_vector.size(); i++) {
        midcode t = midcode_vector.at(i);
        if (t.get_first_pos() == Write_string_midcode) {
            push_to_instruction_vector(name + to_string(sting_name_index++) + string(":"), string(".asciiz"),
                                       string("\"") + t.get_second_pos() + string("\""));
            string_buffer.push_back(t.get_second_pos());
            size += deal_size(t.get_second_pos()) + 1;
        }
    }
    if (size % 4 != 0) {
        string fix = string("");
        for (int i = 0; i < 3 - size % 4; i++) {
            fix = fix + string("x");
        }
        push_to_instruction_vector(string("fix") + string(":"), string(".asciiz"),
                                   string("\"") + fix + string("\""));
    }
}

int find_stringFromStringBuffer(string str) {
    for (int i = 0; i < string_buffer.size(); i++) {
        if (str == string_buffer.at(i)) {
            return i;
        }
    }
}

void alloc() {
    string function_name = now_func;
    vector<signal_table_element> var_vector = table.get_func_var(function_name);
    for (int i = 0; i < var_vector.size(); i++) {
        signal_table_element e = var_vector.at(i);
        string var_name = e.name;
        if (!register_s_vector_full()) {
            string reg_name = alloc_s_Reg2Var(var_name, function_name);
            if(e.kind == param_str){
                load_Var2Register(e.name,reg_name,now_func);
            }
        }
    }
}

void Main_func_define() {
    //cout_note("Main_define");
    now_func = string("main");
    push_to_instruction_vector(string("main:"));
    for (int i = 0; i < GRF.size(); i++) {
        GRF[i].set_is_unused();
    }
    //cout_note("Main_define_end");
}

void Var_declare(string offset) {
    //存储临时变量顶端在$v1寄存器
    // subi $sp $sp offset
    var_count = offset;
    push_to_instruction_vector(subi_mips, stack_reg, stack_reg, offset);
    // add $v1 $sp 0
    push_to_instruction_vector(addi_mips, var_base_address_reg, stack_reg, zero_str);
    alloc();
}

void LoadVar_to_special_reg(string var_name, string reg, string now_func) {
    if (!var_in_Svector(var_name, now_func)) {
        load_Var2Register(var_name, reg, now_func);
    } else {
        string rs = find_var_from_Svector(var_name, now_func);
        push_to_instruction_vector(move_mips, reg, rs);
    }
}

string LoadVar(string var_name, string else_reg, string now_func) {
    string rs;
    if (!var_in_Svector(var_name, now_func)) {
        rs = else_reg;
        load_Var2Register(var_name, rs, now_func);
        return rs;
    } else {
        rs = find_var_from_Svector(var_name, now_func);
        return rs;
    }
}

void SaveVar(string var_name, string value_reg, string now_func) {
    if (var_in_Svector(var_name, now_func)) {
        string rs = find_var_from_Svector(var_name, now_func);
        push_to_instruction_vector(move_mips, rs, value_reg);
    } else {
        save_Register2Var(var_name, value_reg, now_func);
    }
}

void calculate_instruction(midcode t) {
    string second_pos = t.get_second_pos();
    string third_pos = t.get_third_pos();
    string fourth_pos = t.get_fourth_pos();
    bool is_temp_3rd = is_temp_midcode(third_pos);
    bool is_temp_4th = is_temp_midcode(fourth_pos);
    string rd, rs, rt;
    bool is_const_3rd = is_CONST(third_pos), is_const_4th = is_CONST(fourth_pos);

    //判断是否需要存储到内存中
    rd = alloc_register(second_pos, register_t_vector);

    if (is_temp_3rd) {
        rs = get_register_name_by_VarName(third_pos);
    } else if (is_const_3rd) {
        rs = rs_reg;
        load_const_to_Register(third_pos, rs_reg);
    } else {
        rs = LoadVar(third_pos, rs_reg, now_func);
    };

    if (is_temp_4th) {
        rt = get_register_name_by_VarName(fourth_pos);
    } else if (is_const_4th) {
        rt = rt_reg;
        load_const_to_Register(fourth_pos, rt_reg);
    } else {
        rt = LoadVar(fourth_pos, rt_reg, now_func);
    }

    push_to_instruction_vector(t.get_first_pos(), rd, rs, rt);
}

void VAT(midcode t) {
    string goal_temp_name = t.get_second_pos();
    string array_name = t.get_third_pos();
    string index = t.get_fourth_pos();
    bool index_is_temp = is_temp_midcode(index);
    bool index_is_const = is_CONST(index);
    bool index_is_ident = is_ident(index);
    string rt, rs, rd = rd_reg;
    //lw rt (rd+rs)
    load_var_address2Register(array_name, rd, now_func);
    if (index_is_const) {
        rs = rs_reg;
        load_const_to_Register(index, rs);
    } else if (index_is_temp) {
        rs = get_register_name_by_VarName(index);
    } else if (index_is_ident) {
        rs = LoadVar(index, rs_reg, now_func);
    }
    push_to_instruction_vector(mul_mips, rs_reg, rs, string("4"));
    rt = alloc_register(goal_temp_name, register_t_vector);
    push_to_instruction_vector(add_mips, rd, rd, rs_reg);
    push_to_instruction_vector(load_word_mips, rt, bracket_l + rd + bracket_r);
}

void read_mips(midcode t) {
    string value_name = t.get_second_pos();
    string offset = to_string(table.get_offset(value_name, now_func));
    string base;
    if (t.get_first_pos() == Read_int_midcode) {
        // li $v0 5
        push_to_instruction_vector(li_mips, reg_v0, string("5"));
    } else {
        // li $v0 12
        push_to_instruction_vector(li_mips, reg_v0, string("12"));
    }
    syscall();
    // sw $v0 offset($base)
//    push_to_instruction_vector(save_word_mips, reg_v0, offset, bracket_l + base + bracket_r);
    SaveVar(value_name, reg_v0, now_func);
}

void write_mips(midcode t) {
    string second = t.get_second_pos();
    bool is_temp = is_temp_midcode(second);
    bool ident = is_ident(second);
    bool is_global = table.get_is_global(second, now_func);
    if (t.get_first_pos() == Write_int_midcode) {
        // li $v0 1
        push_to_instruction_vector(li_mips, reg_v0, string("1"));
        if (is_temp) {
            // addi $a0 $t1 0
            string temp_reg = get_register_name_by_VarName(second);
            push_to_instruction_vector(addi_mips, reg_a0, temp_reg, zero_str);
        } else if (ident) {
            // lw $a0 offset($base)
            LoadVar_to_special_reg(second, reg_a0, now_func);
        } else {
            // li $a0 const
            push_to_instruction_vector(li_mips, reg_a0, second);
        }
    } else if (t.get_first_pos() == Write_char_midcode) {
        // li $v0 11
        push_to_instruction_vector(li_mips, reg_v0, string("11"));
        if (is_temp) {
            // addi $a0 $t1 0
            string temp_reg = get_register_name_by_VarName(second);
            push_to_instruction_vector(addi_mips, reg_a0, temp_reg, zero_str);
        } else if (ident) {
            // lw $a0 offset($base)
            LoadVar_to_special_reg(second, reg_a0, now_func);
        } else {
            // li $a0 const
            push_to_instruction_vector(li_mips, reg_a0, second);
        }
    } else if (t.get_first_pos() == Write_string_midcode) {
        // li $v0 4
        push_to_instruction_vector(li_mips, reg_v0, string("4"));
        int index = find_stringFromStringBuffer(t.get_second_pos());
        // la $a0 str+to_string(index)
        push_to_instruction_vector(la_mips, reg_a0, string("str") + to_string(index));
    }
    syscall();
}


void assign_mips(midcode t) {
    string var_name = t.get_second_pos();
    string value = t.get_fourth_pos();
    bool is_const = is_CONST(value);
    bool is_var = is_ident(value);
    bool is_temp = is_temp_midcode(value);
    string rs = rs_reg;

    if (is_const) {
        load_const_to_Register(value, rs);
    } else if (is_var) {
        rs = LoadVar(value, rs_reg, now_func);
    } else if (is_temp) {
        rs = get_register_name_by_VarName(value);
    }
    SaveVar(var_name, rs, now_func);
}

void assign_array_mips(midcode t) {
    string array_name = t.get_second_pos();
    string index = t.get_third_pos();
    string value = t.get_fourth_pos();
    //sw rt, rs(rd)
    string rs, rt, rd = rd_reg;
    bool value_is_temp = is_temp_midcode(value);
    bool value_is_const = is_CONST(value);
    bool value_is_ident = is_ident(value);
    bool index_is_temp = is_temp_midcode(index);
    bool index_is_const = is_CONST(index);
    bool index_is_ident = is_ident(index);
    if (value_is_temp) {
        rt = get_register_name_by_VarName(value);
    } else if (value_is_ident) {
        rt = LoadVar(value, rt_reg, now_func);
    } else if (value_is_const) {
        rt = rt_reg;
        load_const_to_Register(value, rt);
    }
    if (index_is_temp) {
        rs = get_register_name_by_VarName(index);
    } else if (index_is_ident) {
        rs = LoadVar(index, rs_reg, now_func);
    } else if (index_is_const) {
        rs = rs_reg;
        load_const_to_Register(index, rs);
    }
    push_to_instruction_vector(mul_mips, rs_reg, rs, string("4"));
    load_var_address2Register(array_name, rd, now_func);
    push_to_instruction_vector(add_mips, rd, rd, rs_reg);
    push_to_instruction_vector(save_word_mips, rt, bracket_l + rd + bracket_r);
}

void enter_mips(midcode t) {
    int en = '\n';
    push_to_instruction_vector(li_mips, reg_v0, to_string(11));
    push_to_instruction_vector(li_mips, reg_a0, to_string(en));
    syscall();
}

void label_mips(midcode t) {
    push_to_instruction_vector(string(""));
    push_to_instruction_vector(string(""));
    push_to_instruction_vector(t.get_second_pos() + string(":"));
}

void b_instruction(midcode t) {
    string third_pos = t.get_third_pos();
    string fourth_pos = t.get_fourth_pos();
    bool is_const_3rd = is_CONST(third_pos);
    bool is_const_4th = is_CONST(fourth_pos);
    bool is_temp_3 = is_temp_midcode(third_pos);
    bool is_temp_4 = is_temp_midcode(fourth_pos);
    bool is_ident_3 = is_ident(third_pos);
    bool is_ident_4 = is_ident(fourth_pos);
    string rs = rs_reg, rt = rt_reg;
    if (is_const_3rd) {
        load_const_to_Register(third_pos, rs);
    } else if (is_temp_3) {
        rs = get_register_name_by_VarName(third_pos);
    } else if (is_ident_3) {
        rs = LoadVar(third_pos, rs_reg, now_func);
    }
    if (is_const_4th) {
        load_const_to_Register(fourth_pos, rt);
    } else if (is_temp_4) {
        rt = get_register_name_by_VarName(fourth_pos);
    } else if (is_ident_4) {
        rt = LoadVar(fourth_pos, rt_reg, now_func);
    }
    push_to_instruction_vector(t.get_first_pos(), rs, rt, t.get_second_pos());
}

void jump_mips(midcode t) {
    string goal_label = t.get_third_pos();
    push_to_instruction_vector(j_mips, goal_label);
}

void save_sence(midcode t) {
    string func_name = t.get_fourth_pos();
    vector<Register> t_reg_vector;
    function_reg_buffer.push_back(t_reg_vector);
    for (int i = 0; i < register_s_vector.size(); i++) {
        int reg_index = register_s_vector.at(i);
        if (GRF.at(reg_index).get_is_used()) {
            function_reg_buffer.at(function_reg_buffer_index).push_back(GRF.at(reg_index));
            // subi $sp $sp 4
            push_to_instruction_vector(subi_mips, stack_reg, stack_reg, string("4"));
            // sw $t0 ($sp)
            push_to_instruction_vector(save_word_mips, GRF.at(reg_index).get_name(), bracket_l + stack_reg + bracket_r);
        }
    }
    for (int i = 0; i < register_t_vector.size(); i++) {
        int reg_index = register_t_vector.at(i);
        if (GRF.at(reg_index).get_is_used()) {
            function_reg_buffer.at(function_reg_buffer_index).push_back(GRF.at(reg_index));
            // subi $sp $sp 4
            push_to_instruction_vector(subi_mips, stack_reg, stack_reg, string("4"));
            // sw $t0 ($sp)
            push_to_instruction_vector(save_word_mips, GRF.at(reg_index).get_name(), bracket_l + stack_reg + bracket_r);
        }
    }
    // subi $sp $sp 4
    push_to_instruction_vector(subi_mips, stack_reg, stack_reg, string("4"));
    // sw $v1 ($sp)
    push_to_instruction_vector(save_word_mips, var_base_address_reg, bracket_l + stack_reg + bracket_r);
    // subi $sp $sp 4
    push_to_instruction_vector(subi_mips, stack_reg, stack_reg, string("4"));
    // sw $ra ($sp)
    push_to_instruction_vector(save_word_mips, ra_reg, bracket_l + stack_reg + bracket_r);
    function_reg_buffer_index++;
}

void recover_sence() {
    function_reg_buffer_index--;
    vector<Register> t_reg_vector = function_reg_buffer.at(function_reg_buffer_index);
    function_reg_buffer.pop_back();
    for (int i = 0; i < register_t_vector.size(); i++) {
        int reg_index = register_t_vector.at(i);
        GRF.at(reg_index).set_is_unused();
    }
    for (int i = 0; i < register_s_vector.size(); i++) {
        int reg_index = register_s_vector.at(i);
        GRF.at(reg_index).set_is_unused();
    }
    // lw $ra ($sp)
    push_to_instruction_vector(load_word_mips, ra_reg, bracket_l + stack_reg + bracket_r);
    // addi $sp $sp 4
    push_to_instruction_vector(addi_mips, stack_reg, stack_reg, string("4"));
    // lw $v0 ($sp)
    push_to_instruction_vector(load_word_mips, var_base_address_reg, bracket_l + stack_reg + bracket_r);
    // addi $sp $sp 4
    push_to_instruction_vector(addi_mips, stack_reg, stack_reg, string("4"));
    for (int i = t_reg_vector.size() - 1; i >= 0; i--) {
        Register reg = t_reg_vector.at(i);
        int reg_index = get_register_index(reg.get_name());
        GRF[reg_index] = reg;
        // lw $t0 ($sp)
        push_to_instruction_vector(load_word_mips, reg.get_name(), bracket_l + stack_reg + bracket_r);
        // addi $sp $sp 4
        push_to_instruction_vector(addi_mips, stack_reg, stack_reg, string("4"));
    }
}

void push_mips(midcode t) {
    int count;
//    param_count += 4;
    string value = t.get_fourth_pos();
    string offset = t.get_third_pos();
    bool value_is_temp = is_temp_midcode(value);
    bool value_is_const = is_CONST(value);
    bool value_is_ident = is_ident(value);
    string rt = rt_reg;
    // sw rt offset($sp)
    // prepare rt reg
    if (value_is_temp) {
        rt = get_register_name_by_VarName(value);
    } else if (value_is_ident) {
        rt = LoadVar(value, rt_reg, now_func);
    } else if (value_is_const) {
        load_const_to_Register(value, rt);
    }
    push_to_instruction_vector(subi_mips, stack_reg, stack_reg, string("4"));
    push_to_instruction_vector(save_word_mips, rt, bracket_l + stack_reg + bracket_r);
}

void function_define(midcode t) {
    string func_name = t.get_second_pos();
    now_func = func_name;
    push_to_instruction_vector(func_name + string(":"));
    for (int i = 0; i < GRF.size(); i++) {
        GRF[i].set_is_unused();
    }
}

void call_mips(midcode t) {
    for (int i = 0; i < GRF.size(); i++) {
        GRF.at(i).set_is_unused();
    }

    string return_value = t.get_second_pos();
    string func_name = t.get_third_pos();
    int param_count = 4 * func_table.get_func_param_count(func_name);
    push_to_instruction_vector(addi_mips, stack_reg, stack_reg, to_string(param_count));
    push_to_instruction_vector(jal_mips, func_name);
    recover_sence();
    if (return_value != null_midcode) {
        string temp_reg = alloc_register(return_value, register_t_vector);
        push_to_instruction_vector(move_mips, temp_reg, save_return_value_reg);
    }
}

void function_end_mips(midcode t) {
    // addi $sp $sp var_count
    push_to_instruction_vector(addi_mips, stack_reg, stack_reg, var_count);
    // jr ra
    push_to_instruction_vector(jr_mips, ra_reg);
}

void return_mips(midcode t) {
    string return_value = t.get_second_pos();
    if (return_value != null_midcode) {
        bool return_value_is_const = is_CONST(return_value);
        bool return_value_is_temp = is_temp_midcode(return_value);
        bool return_value_is_ident = is_ident(return_value);
        // move $v1 return_value
        if (return_value_is_const) {
            load_const_to_Register(return_value, save_return_value_reg);
        } else if (return_value_is_temp) {
            string rs = get_register_name_by_VarName(return_value);
            push_to_instruction_vector(move_mips, save_return_value_reg, rs);
        } else if (return_value_is_ident) {
            LoadVar_to_special_reg(return_value, save_return_value_reg, now_func);
        }
    }
    // addi $sp $sp var_count
    push_to_instruction_vector(addi_mips, stack_reg, stack_reg, var_count);
    // jr ra
    push_to_instruction_vector(jr_mips, ra_reg);
}

void pre_deal() {
    for (int i = 0; i < func_table.get_func_table_size(); i++) {
        string func_name = func_table.get(i).name;
        int base = table.var_in_function_offset(func_name);
        for (int j = func_table.get(i).param_list.size() - 1; j >= 0; j--) {
            int offset = base + 4 * (func_table.get(i).param_list.size() - j - 1);
            table.change_offset(func_table.get(i).param_list.at(j).second, func_name, offset);
        }
    }
}

void produce_mips() {
    pre_deal();
    create_GRF();
    distribute_memory_for_string();
    for (int i = 0; i < midcode_vector.size(); i++) {
        midcode t = midcode_vector.at(i);
        if (debug) {
            print_midcode(t);
        }
        string first_pos = t.get_first_pos();
        if (first_pos == Call_midcode) {
            param_count = 0;
        }
        if (i == 0 && first_pos == Var_declare_midcode) {
            string offset = t.get_fourth_pos();
            push_to_instruction_vector(string("global_var:"), string(".space"), offset);
            push_to_instruction_vector(string(".text"));
            push_to_instruction_vector(la_mips, global_var_base_address_reg, string("global_var"));
        }

        if (first_pos == Main_define_declare_midcode && i == 0) {
            push_to_instruction_vector(string(".text"));
        }
        if (i == 0) {
            push_to_instruction_vector(jal_mips, main_str);
            push_to_instruction_vector(li_mips, reg_v0, string("10"));
            syscall();
        }
        if (first_pos == Main_define_declare_midcode) {
            Main_func_define();
        } else if (first_pos == Var_declare_midcode && i != 0) {
            Var_declare(t.get_fourth_pos());
        } else if (first_pos == ADD_midcode
                   || first_pos == SUB_midcode
                   || first_pos == MUL_midcode
                   || first_pos == DIV_midcode) {
            calculate_instruction(t);
        } else if (first_pos == VAT_midcode) {
            VAT(t);
        } else if (first_pos == Assign_midcode) {
            assign_mips(t);
        } else if (first_pos == Assign_array_midcode) {
            assign_array_mips(t);
        } else if (first_pos == Write_string_midcode
                   || first_pos == Write_char_midcode
                   || first_pos == Write_int_midcode) {
            write_mips(t);
        } else if (first_pos == Read_int_midcode || first_pos == Read_char_midcode) {
            read_mips(t);
        } else if (first_pos == Enter_midcode) {
            enter_mips(t);
        } else if (first_pos == Lable_midcode) {
            label_mips(t);
        } else if (first_pos == Equal_midcode
                   || first_pos == Not_Equal_midcode
                   || first_pos == Greater_than_midcode
                   || first_pos == Less_than_midcode
                   || first_pos == Greater_or_Equal_midcode
                   || first_pos == Less_or_Equal_midcode) {
            b_instruction(t);
        } else if (first_pos == Call_midcode) {
            call_mips(t);
        } else if (first_pos == Save_sence_midcode) {
            save_sence(t);
        } else if (first_pos == Push_param_midcode) {
            push_mips(t);
        } else if (first_pos == Function_return_midcode) {
            return_mips(t);
        } else if (first_pos == Function_end_midcode) {
            function_end_mips(t);
        } else if (first_pos == Func_define_midcode) {
            function_define(t);
        } else if (first_pos == Jump_midcode) {
            jump_mips(t);
        }
    }
}