#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include "expr_lexer.h"

#define RETURN_TOKEN(tk)    \
    do{                     \
        lexeme = next;      \
        next = file.get();  \
        return tk;          \
    }while(0)


void Lexer::next_char(){
    next = file.get();
}

bool Lexer::equal_to_check(){
    next_char();
    if(next == '='){
        lexeme += '=';
        next_char();
        return true;
    }
    return false;
}

void Lexer::Loop(std::function<bool(char)> function){
    while(function(next)){
        lexeme += next;
        next_char();
    }
}

Tokens Lexer::get_Token(){
    lexeme = "";
    if(space_count >= 0 && space_count < block_spaces.top()){
        block_spaces.pop();
        if(space_count <= block_spaces.top()){
            lexeme = " ";
            return Tokens::DEDENT_TOKEN;
        }else{
            cerr << "Lexer Error on line " << line << ", Unexpected identation." << endl;
            exit(1);
        }
    }
    switch(next){
        case '+': RETURN_TOKEN(Tokens::ADD_TOKEN);
        case '-': RETURN_TOKEN(Tokens::SUB_TOKEN);
        case '/': RETURN_TOKEN(Tokens::DIV_TOKEN);
        case '%': RETURN_TOKEN(Tokens::MOD_TOKEN);
        case '(': RETURN_TOKEN(Tokens::PAR_IZQ_TOKEN);
        case ')': RETURN_TOKEN(Tokens::PAR_DER_TOKEN);
        case '{': RETURN_TOKEN(Tokens::OPEN_BRACKET_TOKEN);
        case '}': RETURN_TOKEN(Tokens::CLOSED_BRACKET_TOKEN);
        case '[': RETURN_TOKEN(Tokens::ARRAY_OPEN_TOKEN);
        case ']': RETURN_TOKEN(Tokens::ARRAY_CLOSED_TOKEN);
        case ':': RETURN_TOKEN(Tokens::COLON_TOKEN);
        case ',': RETURN_TOKEN(Tokens::COMMA_TOKEN);
        case ' ': next_char(); return get_Token();
        case EOF: RETURN_TOKEN(Tokens::EOF_TOKEN);
        case '\n': 
            //cout <<"fuck"<<endl;
            next_char();
            space_count = 0;
            line++;
            while(next == ' '){
                space_count++;  
                next_char();
            }if(next == EOF){
                return Tokens::EOF_TOKEN;
            }else if(space_count > block_spaces.top()){
                block_spaces.push(space_count);
                lexeme = " ";
                space_count = -1;
                return Tokens::INDENT_TOKEN;
            }else if(space_count < block_spaces.top()){
                return get_Token();
            }else{
                return Tokens::NEW_LINE_TOKEN;
            }
        default:
            if(isdigit(next)){
                Loop([](char ch){ return isdigit(ch);});
                return Tokens::NUM_TOKEN;
            }else if(next == '"'){
                lexeme = "\"";
                next_char();
                if(next == '"'){
                    lexeme += "\"";
                    next_char();
                    if(next == '"'){
                        lexeme += "\"";
                        next_char();
                        Loop([](char ch){ return ch != '"' && ch != EOF;});
                        lexeme += "\"";
                        next_char();
                        if(next == '"'){
                            lexeme += "\"";
                            next_char();
                            if(next == '"'){
                                lexeme += "\"";
                                next_char();
                                if(next == '\n' || next == EOF){
                                    return get_Token();
                                }
                            }
                        }
                    }else{
                        return Tokens::STR_TOKEN;
                    }
                    cerr << "Lexer Error on line " << line << ", Bad comment" << endl;
                    exit(1);
                }else{
                    Loop([](char ch){ return ch != '\n' && ch != '"' && ch != EOF;});
                    if(next ==  '\n' || next == EOF){
                        cerr << "Lexer Error on line " << line << ", Bad string" << endl;
                        exit(1);
                    }
                    lexeme += next;
                    next_char();
                    return Tokens::STR_TOKEN;
                }
            }else if(next == '\''){
                lexeme = "'";
                next_char();
                Loop([](char ch){ return ch != '\n' && ch != '\'' && ch != EOF;});
                if(next ==  '\n' || next == EOF){
                    cerr << "Lexer Error on line " << line << ", Bad string" << endl;
                    exit(1);
                }
                lexeme += next;
                next_char();
                return Tokens::STR_TOKEN;
            }else if(next == '<'){
                lexeme = '<';
                if(equal_to_check()){
                    return Tokens::LESS_EQUAL_TOKEN; 
                }
                return Tokens::LESS_THAN_TOKEN;
            }else if(next == '>'){
                lexeme = '>';
                if(equal_to_check()){
                    return Tokens::MORE_EQUAL_TOKEN; 
                }
                return Tokens::MORE_THAN_TOKEN;
            }else if(next == '='){
                lexeme = '=';
                if(equal_to_check()){
                    return Tokens::EQUAL_EQUAL_TOKEN; 
                }
                return Tokens::EQUAL_TOKEN;
            }else if(next == '!'){
                lexeme = '!';
                if(equal_to_check()){
                    return Tokens::NOT_EQUAL_TOKEN; 
                }
                return Tokens::ERR_TOKEN;
            }else if(next == '*'){
                lexeme = '*';
                next_char();
                if(next == '*'){
                    lexeme += '*';
                    next_char();
                    return Tokens::EXP_TOKEN;
                }
                return Tokens::MULT_TOKEN;
            }else if(isalpha(next) || next == '_'){
                Loop([](char ch){ return isdigit(ch) || isalpha(ch) || ch == '_';});
                if(!lexeme.compare("if")){
                    return Tokens::IF_TOKEN;
                }else if(!lexeme.compare("while")){
                    return Tokens::WHILE_TOKEN;
                }else if(!lexeme.compare("for")){
                    return Tokens::FOR_TOKEN;
                }else if(!lexeme.compare("else")){
                    return Tokens::ELSE_TOKEN;
                }else if(!lexeme.compare("input")){
                    return Tokens::INPUT_TOKEN;
                }else if(!lexeme.compare("print")){
                    return Tokens::PRINT_TOKEN;
                }else if(!lexeme.compare("in")){
                    return Tokens::IN_TOKEN;
                }else if(!lexeme.compare("range")){
                    return Tokens::RANGE_TOKEN;
                }else if(!lexeme.compare("pass")){
                    return Tokens::PASS_TOKEN;
                }else if(!lexeme.compare("and")){
                    return Tokens::AND_TOKEN;
                }else if(!lexeme.compare("or")){
                    return Tokens::OR_TOKEN;
                }else if(!lexeme.compare("True")){
                    return Tokens::TRUE_TOKEN;
                }else if(!lexeme.compare("False")){
                    return Tokens::FALSE_TOKEN;
                }else if(!lexeme.compare("len")){
                    return Tokens::LEN_TOKEN;
                }
                return Tokens::ID_TOKEN;
            }else if(next == '#'){
                next_char();
                Loop([](char ch){ return ch != '\n' && ch != EOF;});
                return get_Token();
            }else{
                cerr << "Lexer Error on line " << line << ", Unexpected Token" << endl;
                exit(1);
            }      
    }
}

string Lexer::get_Lexeme(){
    return lexeme;
}

int Lexer::get_Line(){
    return line;
}