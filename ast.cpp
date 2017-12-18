#include "ast.h"
#include <map>
#include <cmath>
#include <list>

using namespace std;

map <string, Expression*> ints;

string StrExpr::value(){
    return str;
}

int NUMExpr::value(){
    return val;
}

int IDExpr::value(){
    if(ints.find(id) != ints.end()){
        Expression* temp = ints[id];
        if(temp->is_array == 1){
            if(pos->value() == -1){
                cerr << "ID is an array" << endl;
                exit(1);
            }else{
                if(pos->value() < 0){
                    cerr << "Array position cannot be negative" << endl;
                    exit(1);
                }
                Expression* array = ints[id];
                for(int c = 0; c <= pos->value(); c++){
                    if(array == NULL){
                        cerr << "Out of bounds" << endl;
                        exit(1);
                    }
                    if(c == pos->value()){
                        return array->value();
                    }
                    array = (Expression*)array->next;
                }    
            }
        }else{
            return ints[id]->value();
        }
    }
    cerr << "ID not yet defined " << endl;
    exit(1);
}

int AddExpr::value(){
    return l->value() + r->value();
}

int SubExpr::value(){
    return l->value() - r->value();
}

int MultExpr::value(){
    return l->value() * r->value();
}

int DivExpr::value(){
    if(r->value() != 0){
        return l->value() / r->value();
    }
    cerr << "Cannot divide by 0" << endl;
    exit(1);
}

int ModExpr::value(){
    if(r->value() != 0){
        return l->value() % r->value();
    }
    cerr << "Cannot divide by 0" << endl;
    exit(1);
}

int ExpExpr::value(){
    return pow(l->value(), r->value());
}

int LTExpr::value(){
    return l->value() < r->value();
}

int GTExpr::value(){
    return l->value() > r->value();
}

int LTEExpr::value(){
    return l->value() <= r->value();
}

int GTEExpr::value(){
    return l->value() >= r->value();
}

int EQExpr::value(){
    return l->value() == r->value();
}

int NEExpr::value(){
    return l->value() != r->value();
}

int AndExpr::value(){
    if(l->value() > 0){
        if(r->value() > 0){
            return 1;
        }
    }
    return 0;
}

int OrExpr::value(){
    if(l->value() > 0){
        return 1;
    }
    if(r->value() > 0){
        return 1;
    }
    return 0;
}

int TrueExpr::value(){
    return 1;
}

int FalseExpr::value(){
    return 0;
}

int InputExpr::value(){
    string temp = str->value().substr(1, str->value().size() - 2);
    cout << temp;
    int val;
    cin >> val;
    return val;
}

int LenExpr::value(){
    if(ints.find(id) != ints.end()){
        Expression* temp = ints[id];
        if(temp->is_array == 1){
            int c;
            for(c = 0; temp!= NULL ; c++){
                temp=(Expression*)temp->next;
            }
            return c;
        }else{
            cerr << "ID is not an array" << endl;
            exit(1);    
        }
    }
}

void PrintStatement::execute(){
    if(to_print->next == NULL){
        if(to_print->type == 1){
            cout << ((StrExpr*)to_print)->value() << endl;
        }else if(to_print->type == 0){
            cout << ((Expression*)to_print)->value() << endl;
        }else if(to_print->type == 2){
            if(((Expression*)to_print)->value() == 1){
                cout << "True" << endl;
            }else{
                cout << "False" << endl;
            }
        }else{
            Expression* temp;
            if(ints.find(((IDExpr*)to_print)->id) != ints.end()){
                temp = ints[((IDExpr*)to_print)->id];
                if(temp->is_array == 1){
                    cout << "[";
                    cout << ((Expression*)temp)->value();
                    temp = (Expression*)temp->next;
                    while(temp != NULL){
                        cout << ", ";
                        cout << ((Expression*)temp)->value();
                        temp = (Expression*)temp->next;
                    }
                    cout << "]" << endl;
                }else{
                    cout << ((Expression*)temp)->value() << endl;
                }
            }else{
                cerr << "ID not yet defined " << endl;
                exit(1);
            }
        }
    }else{
        string str = "(";
        ExprStr *temp = to_print;
        while(temp != NULL){
            if(temp->type == 1){
                str += ((StrExpr*)temp)->value();
            }else if(temp->type == 0){
                str += std::to_string(((Expression*)temp)->value());
            }else if(temp->type == 2){
                if(((Expression*)temp)->value() == 1){
                    str += "True";
                }else{
                    str += "False";
                }
            }else{
                Expression* temp1;
                if(ints.find(((IDExpr*)temp)->id) != ints.end()){
                    temp1 = ints[((IDExpr*)temp)->id];
                    if(temp1->is_array == 1){
                        str += "[";
                        str += std::to_string(((Expression*)temp1)->value());
                        temp1 = (Expression*)temp1->next;
                        while(temp1 != NULL){
                            str += ", ";
                            str += std::to_string(((Expression*)temp1)->value());
                            temp1 = (Expression*)temp1->next;
                        }
                        str += "]";
                    }else{
                        str +=std::to_string(((Expression*)temp1)->value());
                    }
                }else{
                    cerr << "ID not yet defined " << endl;
                    exit(1);
                }
            }
            if(temp->next != NULL)
                str +=", ";
            temp = temp->next;
        }
        str += ")";
        cout << str << endl;
    }
}

void AssignStatement::execute(){
    if(id->pos->value() == -1){
        if(value->is_array == 0){
            ints[id->id] = new NUMExpr(value->value());
        }else{
            Expression* temp1;
            Expression* temp = new NUMExpr(value->value());
            temp->is_array = 1;
            value = (Expression*)value->next;
            temp1 = temp;
            while(value != NULL){
                temp->next = new NUMExpr(value->value());
                temp = (Expression*)temp->next;
                value = (Expression*)value->next;
            }
            ints[id->id] = temp1;
        }
    }else{
        if(ints.find(id->id) != ints.end()){
            if(ints[id->id]->is_array == 1){
                Expression* array = ints[id->id];
                Expression* before;
                if(id->pos->value() < 0){
                    cerr << "Array position cannot be negative" << endl;
                    exit(1);
                }
                for(int c = 0; c <= id->pos->value(); c++){
                    if(array == NULL){
                        cerr << "Out of bounds" << endl;
                        exit(1);
                    }
                    if(c == id->pos->value()){
                        if(c > 0){
                            Expression* new_node = new NUMExpr(value->value());
                            new_node->next = array->next; 
                            before->next = new_node;
                            break;
                        }
                        if(c == 0){
                            Expression* new_head = new NUMExpr(value->value());
                            new_head->next = array->next;
                            new_head->is_array = 1;
                            ints[id->id] = new_head;
                            break;
                        }
                    }
                    before = array;
                    array = (Expression*)array->next;
                }
            }else{
                cerr << "ID is not an array" << endl;
                exit(1);
            }   
        }else{
            cerr << "Array does not exist" << endl;
            exit(1);
        }
    }
}

void IfStatement::execute(){
    if(condition->value() > 0){
        list<string> local_variables;
        Statement* temp = true_block;
        while(temp != NULL){
            if(temp->is_assign){
                if(ints.find(((AssignStatement*)temp)->id->id) == ints.end()){
                    local_variables.push_front(((AssignStatement*)temp)->id->id);
                }
            }
            temp->execute();
            temp = temp->next;
        }
        for (list<string>::iterator it=local_variables.begin(); it != local_variables.end(); ++it){
            ints.erase(*it);
        }
    }else{
        list<string> local_variables;
        Statement* temp = false_block;
        while(temp != NULL){
            if(temp->is_assign){
                if(ints.find(((AssignStatement*)temp)->id->id) == ints.end()){
                    local_variables.push_front(((AssignStatement*)temp)->id->id);
                }
            }
            temp->execute();
            temp = temp->next;
        }
        for (list<string>::iterator it=local_variables.begin(); it != local_variables.end(); ++it){
            ints.erase(*it);
        }
    }
}

void WhileStatement::execute(){
    list<string> local_variables;
    while(condition->value() > 0){
        Statement* temp = true_block;
        while(temp != NULL){
            if(temp->is_assign){
                if(ints.find(((AssignStatement*)temp)->id->id) == ints.end()){
                    local_variables.push_front(((AssignStatement*)temp)->id->id);
                }
            }
            temp->execute();
            temp = temp->next;
        }
    }
    for (list<string>::iterator it=local_variables.begin(); it != local_variables.end(); ++it){
        ints.erase(*it);
    }
}

void ForStatement::execute(){
    list<string> local_variables;
    AssignStatement* assign = new AssignStatement(variable, min);
    if(ints.find(assign->id->id) == ints.end()){
        local_variables.push_front(assign->id->id);
    }
    assign->execute();
    Expression* condition = new LTExpr(variable, max);
    while(condition->value() > 0){
        Statement* temp = true_block;
        while(temp != NULL){
            if(temp->is_assign){
                if(ints.find(((AssignStatement*)temp)->id->id) == ints.end()){
                    local_variables.push_front(((AssignStatement*)temp)->id->id);
                }
            }
            temp->execute();
            temp = temp->next;
        }
        AddExpr* sum = new AddExpr(variable, new NUMExpr(1));
        assign = new AssignStatement(variable, new NUMExpr(sum->value()));
        assign->execute();
    }
    for (list<string>::iterator it=local_variables.begin(); it != local_variables.end(); ++it){
        ints.erase(*it);
    }
}