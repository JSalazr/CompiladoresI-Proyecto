#ifndef __EXPR_LEXER_H
#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <functional>
#include <stack>

using namespace std;

enum class Tokens{
    NUM_TOKEN = 0,
    ID_TOKEN = 1,
    STR_TOKEN = 2,
    ADD_TOKEN = 3,
    SUB_TOKEN = 4,
    MULT_TOKEN = 5,
    DIV_TOKEN = 6,
    MOD_TOKEN = 7,
    EXP_TOKEN = 8,
    PAR_IZQ_TOKEN = 9,
    PAR_DER_TOKEN = 10,
    COLON_TOKEN = 11,
    COMMA_TOKEN = 12,
    EQUAL_TOKEN = 13,
    LESS_THAN_TOKEN = 14,
    MORE_THAN_TOKEN = 15,
    LESS_EQUAL_TOKEN = 16,
    MORE_EQUAL_TOKEN = 17,
    EQUAL_EQUAL_TOKEN = 18,
    NOT_EQUAL_TOKEN = 19,
    PRINT_TOKEN = 20,
    INPUT_TOKEN = 21,
    WHILE_TOKEN = 22,
    IN_TOKEN = 23,
    RANGE_TOKEN = 24,
    IF_TOKEN = 25,
    FOR_TOKEN = 26,
    ELSE_TOKEN = 27, 
    PASS_TOKEN = 28,
    AND_TOKEN = 29,
    OR_TOKEN = 30,
    INDENT_TOKEN = 31,
    DEDENT_TOKEN = 32,
    NEW_LINE_TOKEN = 33,
    OPEN_BRACKET_TOKEN = 34,
    CLOSED_BRACKET_TOKEN = 35,
    ARRAY_OPEN_TOKEN = 36,
    ARRAY_CLOSED_TOKEN = 37,
    TRUE_TOKEN = 38,
    FALSE_TOKEN = 39,
    LEN_TOKEN = 40,
    EOF_TOKEN = 41,
    ERR_TOKEN = 999,
};

class Lexer{
public:
    Lexer(ifstream &file) : file(file){
        lexeme = "";
        next_char();
        block_spaces.push(0);
        line = 1;
    }
    string get_Lexeme();
    Tokens get_Token();
    int get_Line();
    void Loop(std::function<bool(char)> function);
    void next_char();
private:
    bool equal_to_check();
    ifstream &file;
    string lexeme;
    stack<int> block_spaces;
    int space_count;
    char next;
    int line;
};

#endif