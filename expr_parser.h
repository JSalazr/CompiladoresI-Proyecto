#ifndef __EXPR_PARSER_H

#include <iostream>
#include <string>
#include <fstream>
#include <functional>
#include "expr_lexer.h"

using namespace std;

class Parser{
public:
    Parser(Lexer &lexer) : lexer(lexer){
        next_token();
    }
    void parse();
private:
    void A();
    void B();
    void C();
    void D(bool full_condition);
    void E();
    void F();
    void G();
    void H();
    void I();
    void J();
    void K();
    void L();
    void M();
    void N(); 
    void O();
    void P();
    void Q();
    void R();
    void S();
    void T();
    void U();
    void V();
    void next_token();
    Lexer &lexer;
    Tokens current_token;
};

#endif