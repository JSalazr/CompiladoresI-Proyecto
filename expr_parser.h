#ifndef __EXPR_PARSER_H

#include <iostream>
#include <string>
#include <fstream>
#include <functional>
#include "expr_lexer.h"
#include "ast.h"

using namespace std;

class Parser{
public:
    Parser(Lexer &lexer) : lexer(lexer){
        next_token();
    }
    void parse();
private:
    Statement* A();
    StrExpr* B();
    Expression* C();
    Expression* D();
    Expression* E();
    Expression* F();
    Statement* G();
    Expression* H();
    Statement* I();
    StrExpr* J();
    Expression* K();
    Statement* M();
    Expression* O();
    void P();
    Expression* Q();
    void R();
    Expression* S();
    Expression* T();
    Expression* V();
    void next_token();
    Lexer &lexer;
    Tokens current_token;
    string current_lexeme;
};

#endif