#ifndef __AST_H
#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <functional>

using namespace std;

class ExprStr{
public:
    virtual void to_string() = 0;
    ExprStr* next;
    char type;
};

class StrExpr: public ExprStr{
public:
    StrExpr(string str){
        this->str = str;
        next = NULL;
        type = 1;
    }

    void to_string(){
        cout<< "STR: " << str << endl;
    }

    string value();

    string str;
};

class Expression: public ExprStr{
public:
    virtual int value() = 0;
    Expression* next_num;
    char is_array;
};

class NUMExpr: public Expression{
public:
    NUMExpr(int val){
        this->val = val;
        next = NULL;
        type = 0;
        is_array = 0;
    }
    void to_string(){
        cout << "NUM: "<< val;
        if(next != NULL){
            cout <<", ";
            next->to_string();
        }
    }
    int value();
    int val;
};

class IDExpr: public Expression{
public:
    IDExpr(string id){
        this->id = id;
        next = NULL;
        type = 3;
        pos = new NUMExpr(-1);
        is_array = 0;
    }
    void to_string(){
        cout << "ID: "<< id;
        if(next != NULL){
            cout <<", ";
            next->to_string();
        }
    }
    int value();
    string id;
    Expression* pos;
};

class AddExpr: public Expression{
public:
    AddExpr(Expression *l, Expression *r){
        this->l = l;
        this->r = r;
        next = NULL;
        type = 0;
        is_array = 0;
    }
    void to_string(){
        cout << "Add: {";
        l->to_string();
        cout << ", ";
        r->to_string();
        cout << "}";
        if(next != NULL){
            cout <<", ";
            next->to_string();
        }
    }
    int value();
    Expression *l; 
    Expression *r;
};

class SubExpr: public Expression{
public:
    SubExpr(Expression *l, Expression *r){
        this->l = l;
        this->r = r;
        next = NULL;
        type = 0;
        is_array = 0;
    }
    void to_string(){
        cout << "Sub: {";
        l->to_string();
        cout << ", ";
        r->to_string();
        cout << "}";
        if(next != NULL){
            cout <<", ";
            next->to_string();
        }
    }
    int value();
    Expression *l; 
    Expression *r;
};

class MultExpr: public Expression{
public:
    MultExpr(Expression *l, Expression *r){
        this->l = l;
        this->r = r;
        next = NULL;
        type = 0;
        is_array = 0;
    }
    void to_string(){
        cout << " Mult: {";
        l->to_string();
        cout << ", ";
        r->to_string();
        cout << "}";
        if(next != NULL){
            cout <<", ";
            next->to_string();
        }
    }
    int value();
    Expression *l; 
    Expression *r;
};

class DivExpr: public Expression{
public:
    DivExpr(Expression *l, Expression *r){
        this->l = l;
        this->r = r;
        next = NULL;
        type = 0;
        is_array = 0;
    }
    void to_string(){
        cout << "Div: {";
        l->to_string();
        cout << ", ";
        r->to_string();
        cout << "}";
        if(next != NULL){
            cout <<", ";
            next->to_string();
        }
    }
    int value();
    Expression *l; 
    Expression *r;
};

class ModExpr: public Expression{
public:
    ModExpr(Expression *l, Expression *r){
        this->l = l;
        this->r = r;
        next = NULL;
        type = 0;
        is_array = 0;
    }
    void to_string(){
        cout << "Mod: {";
        l->to_string();
        cout << ", ";
        r->to_string();
        cout << "}";
        if(next != NULL){
            cout <<", ";
            next->to_string();
        }
    }
    int value();
    Expression *l; 
    Expression *r;
};

class ExpExpr: public Expression{
public:
    ExpExpr(Expression *l, Expression *r){
        this->l = l;
        this->r = r;
        next = NULL;
        type = 0;
        is_array = 0;
    }
    void to_string(){
        cout << "Exp: {";
        l->to_string();
        cout << ", ";
        r->to_string();
        cout << "}";
        if(next != NULL){
            cout <<", ";
            next->to_string();
        }
    }
    int value();
    Expression *l; 
    Expression *r;
};

class InputExpr: public Expression{
public:
    InputExpr(StrExpr *str){
        this->str = str;
        next = NULL;
        type = 0;
        is_array = 0;
    }
    void to_string(){
        cout << "Input";
        if(next != NULL){
            cout <<", ";
            next->to_string();
        }
    }
    int value();
    StrExpr* str;
};

class LenExpr: public Expression{
public:
    LenExpr(string id){
        this->id = id;
        next = NULL;
        type = 0;
        is_array = 0;
    }
    void to_string(){
        cout << "Length{ ID:" << id << "}";
        if(next != NULL){
            cout <<", ";
            next->to_string();
        }
    }
    int value();
    string id;
};

class LTExpr: public Expression{
public:
    LTExpr(Expression *l, Expression *r){
        this->l = l;
        this->r = r;
        next = NULL;
        type = 2;
        is_array = 0;
    }
    void to_string(){
        cout << "Less Than: {";
        l->to_string();
        cout << ", ";
        r->to_string();
        cout << "}";
        if(next != NULL){
            cout <<", ";
            next->to_string();
        }
    }
    int value();
    Expression *l; 
    Expression *r;
};

class GTExpr: public Expression{
public:
    GTExpr(Expression *l, Expression *r){
        this->l = l;
        this->r = r;
        next = NULL;
        type = 2;
        is_array = 0;
    }
    void to_string(){
        cout << "Greater Than: {";
        l->to_string();
        cout << ", ";
        r->to_string();
        cout << "}";
        if(next != NULL){
            cout <<", ";
            next->to_string();
        }
    }
    int value();
    Expression *l; 
    Expression *r;
};

class LTEExpr: public Expression{
public:
    LTEExpr(Expression *l, Expression *r){
        this->l = l;
        this->r = r;
        next = NULL;
        type = 2;
        is_array = 0;
    }
    void to_string(){
        cout << "Less Than Equal to: {";
        l->to_string();
        cout << ", ";
        r->to_string();
        cout << "}";
        if(next != NULL){
            cout <<", ";
            next->to_string();
        }
    }
    int value();
    Expression *l; 
    Expression *r;
};

class GTEExpr: public Expression{
public:
    GTEExpr(Expression *l, Expression *r){
        this->l = l;
        this->r = r;
        next = NULL;
        type = 2;
    }
    void to_string(){
        cout << "Greater Than Equal to: {";
        l->to_string();
        cout << ", ";
        r->to_string();
        cout << "}";
        if(next != NULL){
            cout <<", ";
            next->to_string();
        }
    }
    int value();
    Expression *l; 
    Expression *r;
};

class EQExpr: public Expression{
public:
    EQExpr(Expression *l, Expression *r){
        this->l = l;
        this->r = r;
        next = NULL;
        type = 2;
    }
    void to_string(){
        cout << "Equal to: {";
        l->to_string();
        cout << ", ";
        r->to_string();
        cout << "}";
        if(next != NULL){
            cout <<", ";
            next->to_string();
        }
    }
    int value();
    Expression *l; 
    Expression *r;
};

class NEExpr: public Expression{
public:
    NEExpr(Expression *l, Expression *r){
        this->l = l;
        this->r = r;
        next = NULL;
        type = 2;
    }
    void to_string(){
        cout << "Not Equal to: {";
        l->to_string();
        cout << ", ";
        r->to_string();
        cout << "}";
        if(next != NULL){
            cout <<", ";
            next->to_string();
        }
    }
    int value();
    Expression *l; 
    Expression *r;
};

class AndExpr: public Expression{
public:
    AndExpr(Expression *l, Expression *r){
        this->l = l;
        this->r = r;
        next = NULL;
        type = 2;
    }
    void to_string(){
        cout << "And: {";
        l->to_string();
        cout << ", ";
        r->to_string();
        cout << "}";
        if(next != NULL){
            cout <<", ";
            next->to_string();
        }
    }
    int value();
    Expression *l; 
    Expression *r;
};

class OrExpr: public Expression{
public:
    OrExpr(Expression *l, Expression *r){
        this->l = l;
        this->r = r;
        next = NULL;
        type = 2;
    }
    void to_string(){
        cout << "Or: {";
        l->to_string();
        cout << ", ";
        r->to_string();
        cout << "}";
        if(next != NULL){
            cout <<", ";
            next->to_string();
        }
    }
    int value();
    Expression *l; 
    Expression *r;
};

class TrueExpr: public Expression{
public:
    TrueExpr(){
        next = NULL;
        type = 2;
    }
    void to_string(){
        cout << "True";
    }
    int value();
};

class FalseExpr: public Expression{
public:
    FalseExpr(){
        next = NULL;
        type = 2;
    }
    void to_string(){
        cout << "False";
    }
    int value();
};

class Statement{
public:
    virtual void to_string() = 0;
    virtual void execute() = 0;
    Statement* next;
};

class PrintStatement: public Statement{
public:
    PrintStatement(StrExpr *to_print){
        this->to_print = to_print;
    }
    void to_string(){
        cout<<"Print{ ";
        to_print->to_string();
        cout<<" }";
    }
    void execute();

    StrExpr* to_print;
};

class AssignStatement: public Statement{
public:
    AssignStatement(IDExpr* id, Expression* value){
        this->id = id;
        this->value = value;
    }
    void to_string(){
        cout<<"Assign{ ";
        id->to_string();
        cout<<", ";
        value->to_string();
        cout<<" }";
    }
    void execute();

    IDExpr* id;
    Expression* value;
};

class IfStatement: public Statement{
public:
    IfStatement(Expression* condition, Statement* true_block, Statement* false_block){
        this->condition = condition;
        this->true_block = true_block;
        this->false_block = false_block;
    }
    void to_string(){
        cout<<"If{ ";
        condition->to_string();
        cout<<" }";
    }
    void execute();

    Expression* condition; 
    Statement* true_block;
    Statement* false_block;
};

class WhileStatement: public Statement{
public:
    WhileStatement(Expression* condition, Statement* true_block){
        this->condition = condition;
        this->true_block = true_block;
    }
    void to_string(){
        cout<<"While{ ";
        condition->to_string();
        cout<<" }";
    }
    void execute();

    Expression* condition; 
    Statement* true_block;
};

class ForStatement: public Statement{
public:
    ForStatement(IDExpr* variable, Expression* min, Expression* max, Statement* true_block){
        this->variable = variable;
        this->min = min;
        this->max = max;
        this->true_block = true_block;
    }
    void to_string(){
        cout<<"For{ ";
        //variable->to_string();
        cout<<", ";
        //min->to_string();
        cout<<", ";
        //max->to_string();
        cout<<", ";
        //true_block->to_string();
        cout<<" }";
    }
    void execute();

    IDExpr* variable;
    Expression* min;
    Expression* max;
    Statement* true_block;
};

#endif