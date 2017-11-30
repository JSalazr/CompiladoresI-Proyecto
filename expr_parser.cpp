#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include "expr_parser.h"

void Parser::next_token(){
    current_token = lexer.get_Token();
}

void Parser::A(){
    if(current_token == Tokens::PRINT_TOKEN){
        next_token();
        if(current_token == Tokens::PAR_IZQ_TOKEN){
            next_token();
            B();
            if(current_token == Tokens::PAR_DER_TOKEN){
                next_token();
                I();
            }else{
                cerr << "Syntax error on line " << lexer.get_Line() << ", Expected ')' but found " << lexer.get_Lexeme() << endl;
                exit(1);
            }
        }else{
            cerr << "Syntax error on line " << lexer.get_Line() << ", Expected '(' but found " << lexer.get_Lexeme() << endl;
            exit(1);
        }
    }else if(current_token == Tokens::ID_TOKEN){
        next_token();
        U();
    }else if(current_token == Tokens::FOR_TOKEN){
        next_token();
        if(current_token == Tokens::ID_TOKEN){
            next_token();
            if(current_token == Tokens::IN_TOKEN){
                next_token();
                if(current_token == Tokens::RANGE_TOKEN){
                    next_token();
                    if(current_token == Tokens::PAR_IZQ_TOKEN){
                        next_token();
                        E();
                        if(current_token == Tokens::COMMA_TOKEN){
                            next_token();
                            E();
                            if(current_token == Tokens::PAR_DER_TOKEN){
                                next_token();
                                if(current_token == Tokens::COLON_TOKEN){
                                    next_token();
                                    if(current_token == Tokens::INDENT_TOKEN){
                                        next_token();
                                        A();
                                        M();
                                    }else{
                                        cerr << "Syntax error on line " << lexer.get_Line() << ", Expected identation" << endl;
                                        exit(1);
                                    }
                                }else{
                                    cerr << "Syntax error on line " << lexer.get_Line() << ", Expected ':' but found " << lexer.get_Lexeme() << endl;
                                    exit(1);
                                }
                            }else{
                                cerr << "Syntax error on line " << lexer.get_Line() << ", Expected ')' but found " << lexer.get_Lexeme() << endl;
                                exit(1);
                            }
                        }else{
                            cerr << "Syntax error on line " << lexer.get_Line() << ", Expected ',' but found " << lexer.get_Lexeme() << endl;
                            exit(1);
                        }
                    }else{
                        cerr << "Syntax error on line " << lexer.get_Line() << ", Expected '(' but found " << lexer.get_Lexeme() << endl;
                        exit(1);
                    }
                }else{
                    cerr << "Syntax error on line " << lexer.get_Line() << ", Expected 'range' but found " << lexer.get_Lexeme() << endl;
                    exit(1);
                }
            }else{
                cerr << "Syntax error on line " << lexer.get_Line() << ", Expected 'in' but found " << lexer.get_Lexeme() << endl;
                exit(1);
            }
        }else{
            cerr << "Syntax error on line " << lexer.get_Line() << ", Expected an identifier but found " << lexer.get_Lexeme() << endl;
            exit(1);
        }
    }else if(current_token == Tokens::IF_TOKEN){
        next_token();
        K();
        if(current_token == Tokens::COLON_TOKEN){
            next_token();
            if(current_token == Tokens::INDENT_TOKEN){
                next_token();
                A();
                N();
            }else{
                cerr << "Syntax error on line " << lexer.get_Line() << ", Expected identation" << endl;
                exit(1);
            }
        }else{
            cerr << "Syntax error on line " << lexer.get_Line() << ", Expected ':' but found " << lexer.get_Lexeme() << endl;
            exit(1);
        }
    }else if(current_token == Tokens::WHILE_TOKEN){
        next_token();
        K();
        if(current_token == Tokens::COLON_TOKEN){
            next_token();
            if(current_token == Tokens::INDENT_TOKEN){
                next_token();
                A();
                M();
            }else{
                cerr << "Syntax error on line " << lexer.get_Line() << ", Expected identation" << endl;
                exit(1);
            }
        }else{
            cerr << "Syntax error on line " << lexer.get_Line() << ", Expected ':' but found " << lexer.get_Lexeme() << endl;
            exit(1);
        }
    }else if(current_token == Tokens::PASS_TOKEN){
        next_token();
        I();
    }else if(current_token == Tokens::NEW_LINE_TOKEN){
        next_token();
        A();
    }else if(current_token == Tokens::EOF_TOKEN){
        return;
    }else{
        cerr << "Syntax error on line " << lexer.get_Line() << ", Expected 'print', 'if', 'while', 'for', 'pass', an identifier or End of File but found " << lexer.get_Lexeme() << endl;
        exit(1);
    }
}

void Parser::B(){
    if(current_token == Tokens::STR_TOKEN){
        next_token();
        J();
    }else{
        D(true);
        J();
    }
}

void Parser::C(){
    if(current_token == Tokens::OPEN_BRACKET_TOKEN){
        next_token();
        S();
        if(current_token == Tokens::CLOSED_BRACKET_TOKEN){
            next_token();
        }else{
            cerr << "Syntax error on line " << lexer.get_Line() << ", Expected '}' but found "<< lexer.get_Lexeme() << endl;
            exit(1);   
        }
    }else{
        E();
    }
}

void Parser::D(bool full_conditon){
    if(current_token == Tokens::TRUE_TOKEN){
        next_token();
        return;
    }else if(current_token == Tokens::FALSE_TOKEN){
        next_token();
    }else{
        E();
        if(current_token == Tokens::LESS_THAN_TOKEN){
            next_token();
            E();
        }else if(current_token == Tokens::MORE_THAN_TOKEN){
            next_token();
            E();
        }else if(current_token == Tokens::MORE_EQUAL_TOKEN){
            next_token();
            E();
        }else if(current_token == Tokens::LESS_EQUAL_TOKEN){
            next_token();
            E();
        }else if(current_token == Tokens::EQUAL_EQUAL_TOKEN){
            next_token();
            E();
        }else if(current_token == Tokens::NOT_EQUAL_TOKEN){
            next_token();
            E();
        }else{
            if(!full_conditon){
                cerr << "Syntax error on line " << lexer.get_Line() << ", Expected condition but found "<< lexer.get_Lexeme() << endl;
                exit(1);
            }
        }
    }
}

void Parser::E(){
    Q();
    while(current_token == Tokens::ADD_TOKEN || current_token == Tokens::SUB_TOKEN){
        if(current_token == Tokens::ADD_TOKEN){
            next_token();
            Q(); 
        }else if(current_token == Tokens::SUB_TOKEN){
            next_token();
            Q(); 
        }
    }
}

void Parser::F(){
    if(current_token == Tokens::NUM_TOKEN){
        next_token();
        return;
    }else if(current_token == Tokens::ID_TOKEN){
        next_token();
        H();
        return;
    }else if(current_token == Tokens::PAR_IZQ_TOKEN){
        next_token();
        E();
        if(current_token == Tokens::PAR_DER_TOKEN){
            next_token();
            return;
        }else{
            cerr << "Syntax error on line " << lexer.get_Line() << ", Expected ')' but found "<< lexer.get_Lexeme() << endl;
            exit(1);
        }
    }else if(current_token == Tokens::INPUT_TOKEN){
        next_token();
        if(current_token == Tokens::PAR_IZQ_TOKEN){
            next_token();
            if(current_token == Tokens::STR_TOKEN){
                next_token();
                if(current_token == Tokens::PAR_DER_TOKEN){
                    next_token();
                    return;
                }else{
                    cerr << "Syntax error on line " << lexer.get_Line() << ", Expected ')' but found "<< lexer.get_Lexeme() << endl;
                    exit(1);
                }
            }else{
                cerr << "Syntax error on line " << lexer.get_Line() << ", Expected a string but found "<< lexer.get_Lexeme() << endl;
                exit(1);
            }
        }else{
            cerr << "Syntax error on line " << lexer.get_Line() << ", Expected '(' but found "<< lexer.get_Lexeme() << endl;
            exit(1);
        }
    }else if(current_token == Tokens::LEN_TOKEN){
        next_token();
        if(current_token == Tokens::PAR_IZQ_TOKEN){
            next_token();
            if(current_token == Tokens::ID_TOKEN){
                next_token();
                if(current_token == Tokens::PAR_DER_TOKEN){
                    next_token();
                    return;
                }else{
                    cerr << "Syntax error on line " << lexer.get_Line() << ", Expected ')' but found "<< lexer.get_Lexeme() << endl;
                    exit(1);
                }
            }else{
                cerr << "Syntax error on line " << lexer.get_Line() << ", Expected a identifier but found "<< lexer.get_Lexeme() << endl;
                exit(1);
            }
        }else{
            cerr << "Syntax error on line " << lexer.get_Line() << ", Expected '(' but found "<< lexer.get_Lexeme() << endl;
            exit(1);
        }
    }else{
        cerr << "Syntax error on line " << lexer.get_Line() << ", Expected a number, an identifier, input or '(' but found "<< lexer.get_Lexeme() << endl;
        exit(1);
    }
}

void Parser::G(){
    if(current_token == Tokens::ELSE_TOKEN){
        next_token();
        if(current_token == Tokens::COLON_TOKEN){
            next_token();
            if(current_token == Tokens::INDENT_TOKEN){
                next_token();
                A();
                R();
            }else{
                cerr << "Syntax error on line " << lexer.get_Line() << ", Expected identation" << endl;
                exit(1);
            }
        }else{
            cerr << "Syntax error on line " << lexer.get_Line() << ", Expected ':' but found " << lexer.get_Lexeme() << endl;
            exit(1);
        }
    }
}

void Parser::H(){
    if(current_token == Tokens::ARRAY_OPEN_TOKEN){
        next_token();
        E();
        if(current_token == Tokens::ARRAY_CLOSED_TOKEN){
            next_token();
        }else{
            cerr << "Syntax error on line " << lexer.get_Line() << ", Expected ']' but found " << lexer.get_Lexeme() << endl;
            exit(1); 
        }
    }
}

void Parser::I(){
    if(current_token == Tokens::NEW_LINE_TOKEN){
        next_token();
        A();
    }
}

void Parser::J(){
    if(current_token == Tokens::COMMA_TOKEN){
        next_token();
        B();
    }
}

void Parser::K(){
    if(current_token == Tokens::PAR_IZQ_TOKEN){
        next_token();
        K();
        if(current_token == Tokens::PAR_DER_TOKEN){
            next_token();
        }else{
            cerr << "Syntax error on line " << lexer.get_Line() << ", Expected ')' but found "<< lexer.get_Lexeme() << endl;
            exit(1);
        }
    }else{
        D(false);
        P();
    }
}

void Parser::M(){
    if(current_token == Tokens::DEDENT_TOKEN){
        next_token();
        A();
    }
}

void Parser::N(){
    if(current_token == Tokens::DEDENT_TOKEN){
        next_token();
        G();
        A();
    }
}

void Parser::O(){
    F();
    if(current_token == Tokens::EXP_TOKEN){
        next_token();
        E();
    }
}

void Parser::P(){
    if(current_token == Tokens::AND_TOKEN){
        next_token();
        K();
    }else if(current_token == Tokens::OR_TOKEN){
        next_token();
        K();
    }else{

    }
}

void Parser::Q(){
    O();
    while(current_token == Tokens::MULT_TOKEN || current_token == Tokens::DIV_TOKEN || current_token == Tokens::MOD_TOKEN){
        if(current_token == Tokens::MULT_TOKEN){
            next_token();
            Q(); 
        }else if(current_token == Tokens::DIV_TOKEN){
            next_token();
            Q(); 
        }else if(current_token == Tokens::MOD_TOKEN){
            next_token();
            Q(); 
        }
    }
}

void Parser::R(){
    if(current_token == Tokens::DEDENT_TOKEN){
        next_token();
    }
}

void Parser::S(){
    E();
    T();
}

void Parser::T(){
    if(current_token == Tokens::COMMA_TOKEN){
        next_token();
        S();
    }
}

void Parser::U(){
    if(current_token == Tokens::EQUAL_TOKEN){
        next_token();
        C();
        I();
    }else if(current_token == Tokens::ARRAY_OPEN_TOKEN){
        next_token();
        E();
        if(current_token == Tokens::ARRAY_CLOSED_TOKEN){
            next_token();
            V();
        }else{
            cerr << "Syntax error on line " << lexer.get_Line() << ", Expected ']' but found " << lexer.get_Lexeme() << endl;
            exit(1); 
        }
    }else{
        cerr << "Syntax error on line " << lexer.get_Line() << ", Expected '=', or '[' but found " << lexer.get_Lexeme() << endl;
        exit(1);
    }
}

void Parser::V(){
    if(current_token == Tokens::EQUAL_TOKEN){
        next_token();
        E();
        I();
    }
}

void Parser::parse(){
    A();
    if(current_token != Tokens::EOF_TOKEN){
    }else{
        cout << "File Parsing ended succesfully" << endl;
    }
}   