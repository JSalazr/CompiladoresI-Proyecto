#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include "expr_parser.h"

void Parser::next_token(){
    current_token = lexer.get_Token();
    current_lexeme = lexer.get_Lexeme();
}

Statement* Parser::A(){
    Statement* to_return;
    if(current_token == Tokens::PRINT_TOKEN){
        next_token();
        if(current_token == Tokens::PAR_IZQ_TOKEN){
            next_token();
            StrExpr* to_print = B();
            if(current_token == Tokens::PAR_DER_TOKEN){
                next_token();
                to_return = new PrintStatement(to_print);
                Statement* next = I();
                to_return->next = next;
                return to_return;
            }else{
                cerr << "Syntax error on line " << lexer.get_Line() << ", Expected ')' but found " << lexer.get_Lexeme() << endl;
                exit(1);
            }
        }else{
            cerr << "Syntax error on line " << lexer.get_Line() << ", Expected '(' but found " << lexer.get_Lexeme() << endl;
            exit(1);
        }
    }else if(current_token == Tokens::ID_TOKEN){
        IDExpr* id = new IDExpr(current_lexeme);
        Expression* expr;
        next_token();
        if(current_token == Tokens::EQUAL_TOKEN){
            next_token();
            expr = C();
        }else if(current_token == Tokens::ARRAY_OPEN_TOKEN){
            next_token();
            Expression* pos = E();
            id->pos = pos;
            if(current_token == Tokens::ARRAY_CLOSED_TOKEN){
                next_token();
                expr = V();
            }else{
                cerr << "Syntax error on line " << lexer.get_Line() << ", Expected ']' but found " << lexer.get_Lexeme() << endl;
                exit(1); 
            }
        }else{
            cerr << "Syntax error on line " << lexer.get_Line() << ", Expected '=', or '[' but found " << lexer.get_Lexeme() << endl;
            exit(1);
        }
        to_return = new AssignStatement(id, expr);
        to_return->next = I();
        return to_return;
    }else if(current_token == Tokens::FOR_TOKEN){
        next_token();
        if(current_token == Tokens::ID_TOKEN){
            IDExpr* variable = new IDExpr(current_lexeme);
            next_token();
            if(current_token == Tokens::IN_TOKEN){
                next_token();
                if(current_token == Tokens::RANGE_TOKEN){
                    next_token();
                    if(current_token == Tokens::PAR_IZQ_TOKEN){
                        next_token();
                        Expression* min = E();
                        if(current_token == Tokens::COMMA_TOKEN){
                            next_token();
                            Expression* max = E();
                            if(current_token == Tokens::PAR_DER_TOKEN){
                                next_token();
                                if(current_token == Tokens::COLON_TOKEN){
                                    next_token();
                                    if(current_token == Tokens::INDENT_TOKEN){
                                        next_token();
                                        Statement* true_block = A();
                                        to_return = new ForStatement(variable, min, max, true_block);
                                        if(current_token == Tokens::DEDENT_TOKEN){
                                            next_token();
                                            to_return->next =  A();
                                        }
                                        return to_return;
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
        Expression *condition = K();
        if(current_token == Tokens::COLON_TOKEN){
            next_token();
            if(current_token == Tokens::INDENT_TOKEN){
                next_token();
                Statement* true_block = A();
                Statement* false_block;
                to_return = new IfStatement(condition, true_block, NULL);
                if(current_token == Tokens::DEDENT_TOKEN){
                    next_token();
                    ((IfStatement*)to_return)->false_block = G();
                    to_return->next = A();
                }
                return to_return;
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
        Expression *condition = K();
        if(current_token == Tokens::COLON_TOKEN){
            next_token();
            if(current_token == Tokens::INDENT_TOKEN){
                next_token();
                Statement* true_block = A();
                to_return = new WhileStatement(condition, true_block);
                if(current_token == Tokens::DEDENT_TOKEN){
                    next_token();
                    to_return->next =  A();
                }
                return to_return;
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
        return I();
    }else if(current_token == Tokens::NEW_LINE_TOKEN){
        next_token();
        return A();
    }else if(current_token == Tokens::EOF_TOKEN || current_token == Tokens::DEDENT_TOKEN){
        return NULL;
    }else{
        cerr << "Syntax error on line " << lexer.get_Line() << ", Expected 'print', 'if', 'while', 'for', 'pass', an identifier or End of File but found " << lexer.get_Lexeme() << endl;
        exit(1);
    }
}

StrExpr* Parser::B(){
    if(current_token == Tokens::STR_TOKEN){
        StrExpr *str = new StrExpr(current_lexeme);
        next_token();
        str->next = J();
        return str;
    }else{
        StrExpr* temp = (StrExpr*)K();
        temp->next = J();
        return temp;
    }
}

Expression* Parser::C(){
    if(current_token == Tokens::ARRAY_OPEN_TOKEN){
        next_token();
        Expression *temp = S();
        temp->is_array = 1;
        if(current_token == Tokens::ARRAY_CLOSED_TOKEN){
            next_token();
            return temp;
        }else{
            cerr << "Syntax error on line " << lexer.get_Line() << ", Expected '}' but found "<< lexer.get_Lexeme() << endl;
            exit(1);   
        }
    }else{
        return E();
    }
}

Expression* Parser::D(){
    if(current_token == Tokens::TRUE_TOKEN){
        next_token();
        return new TrueExpr();
    }else if(current_token == Tokens::FALSE_TOKEN){
        next_token();
        return new FalseExpr();
    }else{
        Expression* temp = E();
        if(current_token == Tokens::LESS_THAN_TOKEN){
            next_token();
            temp = new LTExpr(temp, E());
            return temp;
        }else if(current_token == Tokens::MORE_THAN_TOKEN){
            next_token();
            temp = new GTExpr(temp, E());
            return temp;
        }else if(current_token == Tokens::MORE_EQUAL_TOKEN){
            next_token();
            temp = new GTEExpr(temp, E());
            return temp;
        }else if(current_token == Tokens::LESS_EQUAL_TOKEN){
            next_token();
            temp = new LTEExpr(temp, E());
            return temp;
        }else if(current_token == Tokens::EQUAL_EQUAL_TOKEN){
            next_token();
            temp = new EQExpr(temp, E());
            return temp;
        }else if(current_token == Tokens::NOT_EQUAL_TOKEN){
            next_token();
            temp = new NEExpr(temp, E());
            return temp;
        }else{
            return temp;
        }
    }
}

Expression* Parser::E(){
    Expression *temp = Q();
    while(current_token == Tokens::ADD_TOKEN || current_token == Tokens::SUB_TOKEN){
        if(current_token == Tokens::ADD_TOKEN){
            next_token();
            temp = new AddExpr(temp, Q()); 
        }else if(current_token == Tokens::SUB_TOKEN){
            next_token();
            temp = new SubExpr(temp, Q());
        }
    }
    return temp;
}

Expression* Parser::F(){
    if(current_token == Tokens::NUM_TOKEN){
        Expression* num = new NUMExpr(atoi(current_lexeme.c_str()));
        next_token();
        return num;
    }else if(current_token == Tokens::ID_TOKEN){
        IDExpr* id = new IDExpr(current_lexeme);
        next_token();
        id->pos = H();
        return id;
    }else if(current_token == Tokens::PAR_IZQ_TOKEN){
        next_token();
        Expression * temp = E();
        if(current_token == Tokens::PAR_DER_TOKEN){
            next_token();
            return temp;
        }else{
            cerr << "Syntax error on line " << lexer.get_Line() << ", Expected ')' but found "<< lexer.get_Lexeme() << endl;
            exit(1);
        }
    }else if(current_token == Tokens::INPUT_TOKEN){
        next_token();
        if(current_token == Tokens::PAR_IZQ_TOKEN){
            next_token();
            if(current_token == Tokens::STR_TOKEN){
                StrExpr *str = new StrExpr(current_lexeme);
                next_token();
                if(current_token == Tokens::PAR_DER_TOKEN){
                    next_token();
                    return new InputExpr(str);
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
                string id = current_lexeme;
                next_token();
                if(current_token == Tokens::PAR_DER_TOKEN){
                    next_token();
                    return new LenExpr(id);
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

Statement* Parser::G(){
    if(current_token == Tokens::ELSE_TOKEN){
        next_token();
        if(current_token == Tokens::COLON_TOKEN){
            next_token();
            if(current_token == Tokens::INDENT_TOKEN){
                next_token();
                Statement* to_return = A();
                R();
                return to_return;
            }else{
                cerr << "Syntax error on line " << lexer.get_Line() << ", Expected identation" << endl;
                exit(1);
            }
        }else{
            cerr << "Syntax error on line " << lexer.get_Line() << ", Expected ':' but found " << lexer.get_Lexeme() << endl;
            exit(1);
        }
    }else{
        return NULL;
    }
}

Expression* Parser::H(){
    if(current_token == Tokens::ARRAY_OPEN_TOKEN){
        next_token();
        Expression* to_return = E();
        if(current_token == Tokens::ARRAY_CLOSED_TOKEN){
            next_token();
            return to_return;
        }else{
            cerr << "Syntax error on line " << lexer.get_Line() << ", Expected ']' but found " << lexer.get_Lexeme() << endl;
            exit(1); 
        }
    }
    return new NUMExpr(-1);
}

Statement* Parser::I(){
    if(current_token == Tokens::NEW_LINE_TOKEN){
        next_token();
        return A();
    }
    return NULL;
}

StrExpr* Parser::J(){
    if(current_token == Tokens::COMMA_TOKEN){
        next_token();
        return B();
    }
    return NULL;
}

Expression* Parser::K(){
    if(current_token == Tokens::PAR_IZQ_TOKEN){
        next_token();
        Expression * to_return = K();
        if(current_token == Tokens::PAR_DER_TOKEN){
            next_token();
            return to_return;
        }else{
            cerr << "Syntax error on line " << lexer.get_Line() << ", Expected ')' but found "<< lexer.get_Lexeme() << endl;
            exit(1);
        }
    }else{
        Expression* condition = D();
        while(current_token == Tokens::AND_TOKEN || current_token == Tokens::OR_TOKEN){
            if(current_token == Tokens::AND_TOKEN){
                next_token();
                condition = new AndExpr(condition, K());
            }else if(current_token == Tokens::OR_TOKEN){
                next_token();
                condition = new OrExpr(condition, K());
            }
        }
        return condition;
    }
}

Expression* Parser::O(){
    Expression *temp = F();
    while(current_token == Tokens::EXP_TOKEN){
        next_token();
        temp = new ExpExpr(temp, E());
    }
    return temp;
}

void Parser::P(){
    if(current_token == Tokens::AND_TOKEN){
        next_token();
        K();
    }else if(current_token == Tokens::OR_TOKEN){
        next_token();
        K();
    }
}

Expression* Parser::Q(){
    Expression *temp = O();
    while(current_token == Tokens::MULT_TOKEN || current_token == Tokens::DIV_TOKEN || current_token == Tokens::MOD_TOKEN){
        if(current_token == Tokens::MULT_TOKEN){
            next_token();
            temp = new MultExpr(temp,Q()); 
        }else if(current_token == Tokens::DIV_TOKEN){
            next_token();
            temp = new DivExpr(temp,Q()); 
        }else if(current_token == Tokens::MOD_TOKEN){
            next_token();
            temp = new ModExpr(temp,Q()); 
        }
    }
    return temp;
}

void Parser::R(){
    if(current_token == Tokens::DEDENT_TOKEN){
        next_token();
    }
}

Expression* Parser::S(){
    Expression* temp = E();
    temp->next = T();
    return temp;
}

Expression* Parser::T(){
    if(current_token == Tokens::COMMA_TOKEN){
        next_token();
        return S();
    }
    return NULL;
}

Expression* Parser::V(){
    if(current_token == Tokens::EQUAL_TOKEN){
        next_token();
        return E();
    }
    return NULL;
}

void Parser::parse(){
    Statement* tree = A();
    while(tree != NULL){
        tree->execute();
        tree = tree->next;
    }
    if(current_token != Tokens::EOF_TOKEN){
    }else{
        cout << "File Parsing ended succesfully" << endl;
    }
}   