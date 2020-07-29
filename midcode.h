//
// Created by ym500 on 2019/11/17.
//

#ifndef UNTITLED_MIDCODE_H
#define UNTITLED_MIDCODE_H

#include<string>

using namespace std;

class midcode {
private:
    string first_pos;
    string second_pos;
    string third_pos;
    string fourth_pos;
public:
    midcode(string first, string second, string third, string fourth);

    string get_first_pos();

    string get_second_pos();

    string get_third_pos();

    string get_fourth_pos();
};

void add_MidcodeToVector(string, string, string, string);
#endif //UNTITLED_MIDCODE_H
