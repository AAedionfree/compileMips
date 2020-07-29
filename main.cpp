#include<stdio.h>
#include"split.h"
#include"syntax.h"
#include"function_table.h"
#include "mips.h"

using namespace std;
char codebuffer[MAXSIZE];

int main() {
    char ch;
    int i = 0;
    freopen("testfile.txt", "r", stdin);
    freopen("mips.txt", "w", stdout);
    while ((ch = getchar()) != EOF) {
        codebuffer[i++] = ch;
    }
    codebuffer[i] = EOF;
    while (split());
    getsym();
    process();
    produce_mips();
    print_all();
    return 0;
}
