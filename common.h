#ifndef COMMON_HEADER
#define COMMON_HEADER

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <variant>
#include <unordered_set>
#include <unordered_map>
#include <stack>

enum Token {
    BRACKET_OPEN,       // [
    BRACKET_CLOSE,      // ]
    //INC,                // ++ (postfix increment)
    PARENTHESIS_OPEN,   // (
    PARENTHESIS_CLOSE,  // )
    //DEC,                // -- (postfix decrement)
    MINUS,              // -
    PLUS,               // +
    NOT,                // !
    EXPONENT,           // ^
    MULTIPLY,           // *
    DIVIDE,             // /
    MODULO,             // %
    LESS_THAN,          // <
    LESS_EQUAL,         // <=
    GREATER_THAN,       // >
    GREATER_EQUAL,      // >=
    EQUAL,              // ==
    NOT_EQUAL,          // !=
    AND,                // &&
    OR,                 // ||
    ASSIGN,             // =
    COLON,              // :
    BRACE_OPEN,         // {
    BRACE_CLOSE,        // }
    COMMA,              // ,
    SEMICOLON,          // ;
    STRING_VALUE,       // "wqfas $%asd"
    CHAR_VALUE,         // 'a'
    NUMBER,             // 37
    IDENTIFIER,         // x
    INTEGER,            // integer
    BOOLEAN,            // boolean
    CHAR,               // char
    STRING,             // string
    FALSE,              // false
    TRUE,               // true
    ARRAY,              //array
    ELSE,               //else
    FOR,                //for
    FUNCTION,           //function
    IF,                 //if
    PRINT,              //print
    RETURN,             //return
    VOID,               //void
    WHILE,              //while
    END_OF_FILE         //

};


struct Node
{
    std::string display;
    std::vector<Node*> ptrs;
    std::string nodetag;
    int row, col;
    Node(const std::string type, const std::vector<Node*>& vec, int row, int col) :
        display(type), ptrs(vec), row(row), col(col) {}
    Node() {}

    
};

struct Type{
    std::string type;
    int ndim;
    Type(std::string type, int ndim=0) : type(type), ndim(ndim) {}
    Type() {}
    bool is_array() {return ndim;}
    bool operator==(Type t){
        return type==t.type && ndim==t.ndim;
    }
    bool operator!=(Type t){
        return type!=t.type || ndim!=t.ndim;
    }
};



struct TO_CHECK{
    std::unordered_map<std::string,Type(TO_CHECK::*)(Node*)> to_check;
    std::vector<std::unordered_map<std::string,Type>> ids;
    std::unordered_map<std::string,std::vector<Type>> fun_params;
    Type to_return;
    std::vector<std::string> errors;

    void TYPE_ERROR(Node* n){
        std::ostringstream oss;
        oss << "Type error at "<<n->display<<" (" << n->row << "," << n->col << ")";
        //oss<<" CON HIJOS ";
        // for (int i =0;i<n->ptrs.size();i++){
        //     oss<<n->ptrs[i]->display<<" ";
        // }
        std::string errorMessage = oss.str();
        errors.push_back(errorMessage);
    }

    void ID_ERROR(Node* n){
        std::ostringstream oss;
        if (n->display == "ID")
        {
            oss << "Invalid identifier " << n->ptrs[0]->display << " at "<<" (" << n->row << "," << n->col << ")";
        }
        else
        {
            oss << "Invalid identifier at "<<" (" << n->row << "," << n->col << ")";
        }
        //oss<<" CON HIJOS ";
        // for (int i =0;i<n->ptrs.size();i++){
        //     oss<<n->ptrs[i]->display<<" ";
        // }
        std::string errorMessage = oss.str();
        errors.push_back(errorMessage);
    }

    void displayErrors(){
        std::cout<<"TYPE CHECK: Completed with "<<errors.size()<<" errors\n";
        for (int i=0;i<errors.size();i++){
            std::cout<<errors[i]<<"\n";
        }
    }   

    void check(Node* n){
        //std::cout<<"Estoy en check\n";
        //std::cout<<"Soy nodo: "<<n->display<< "\n";
        if (!n) return;
        auto aux =to_check.find(n->display);
        if (aux!=to_check.end()){
         (  this->*(aux->second))(n);
        }
        else for (int i=0;i<n->ptrs.size();i++){
            check(n->ptrs[i]);
        }
    }

    Type IF(Node*n){
        auto it = to_check.find(n->ptrs[0]->display);
        if (it!=to_check.end()){
            if ((this->*(it->second))(n->ptrs[0])!=Type ("BTYPE")){
                TYPE_ERROR(n);
                return Type("BTYPE");
            }
        }
        else {
            TYPE_ERROR(n);
            return Type("BTYPE");
        }

        for (int i=1;i<n->ptrs.size();i++){
            if (!n->ptrs[i]) break;
            check(n->ptrs[i]);
        }
        return Type ("BTYPE");
    }

    Type ILITERAL(Node*n){
        //std::cout<<n->display<<"En ILITERAL\n";
        return Type("ITYPE");
    }

    Type CLITERAL(Node*n){
        return Type("CTYPE");
    }

    Type BLITERAL(Node*n){
        return Type("BTYPE");
    }

    Type SLITERAL(Node*n){
        return Type("STYPE");
    }

    Type FUNCTION(Node*n){
        Type t (n->ptrs[0]->display);
        if (t.type=="ARRAY") {
            t.ndim = stoi(n->ptrs[0]->ptrs[1]->ptrs[0]->display);
            t.type=n->ptrs[0]->ptrs[0]->display;
        }
        to_return=t;
        if (!ids.back().emplace(n->ptrs[1]->ptrs[0]->display,t).second){
            ID_ERROR(n);
            return Type("BTYPE");
        }
        else {
            //std::cout<<"EN FUNCTION "<<n->ptrs[1]->ptrs[0]->display<<"\n";
            // std::cout<<"SIZE:  "<<n->ptrs.size()<<"\n";
            // std::cout<<"PUNTERO:  "<<n->ptrs[2]<<"\n";
            // std::cout<<"PUNTERO:  "<<n->ptrs[3]<<"\n";
            if (!n->ptrs[2]){
                //std::cout<<"EN FUNCTION SIN PARAMS\n";
                check(n->ptrs[3]);
                return t;
            }
            else {
                std::unordered_map<std::string,Type> id;
                ids.push_back(id);
                std::vector<Type> vec;
                for (int i=0;i< n->ptrs[2]->ptrs.size();i++){
                    Type t (n->ptrs[2]->ptrs[i]->ptrs[0]->display);
                    if (t.type=="ARRAY") {
                        t.ndim = stoi(n->ptrs[2]->ptrs[i]->ptrs[0]->ptrs[1]->ptrs[0]->display);
                        t.type=n->ptrs[2]->ptrs[i]->ptrs[0]->ptrs[0]->display;
                    }
                    if (!ids.back().emplace(n->ptrs[2]->ptrs[i]->ptrs[1]->ptrs[0]->display,t).second){
                        ID_ERROR(n);
                        return Type("BTYPE");
                    }
                    vec.push_back(t);
                }
                fun_params.emplace(n->ptrs[1]->ptrs[0]->display,vec);
                check(n->ptrs[3]);
                ids.pop_back();
                return t;
            }
            
        }
    }

    Type STMTLIST(Node*n){
        if (!n->ptrs[0]) return Type("ITYPE");
        std::unordered_map<std::string,Type> id;
        ids.push_back(id);
        //std::cout<<"SIZE: "<<n->ptrs.size()<<"\n";
        for (int i=0;i<n->ptrs.size();i++){
            check(n->ptrs[i]);
        }
        ids.pop_back();
        return Type("ITYPE");
    }
    Type PROGRAM(Node*n){
        std::unordered_map<std::string,Type> id;
        ids.push_back(id);
        for (int i=0;i<n->ptrs.size();i++){
            check(n->ptrs[i]);
        }
        ids.pop_back();
        return Type("ITYPE");
    }

    Type AND(Node* n){
        Type left,right;
        auto aux1 =to_check.find(n->ptrs[0]->display);
        auto aux2 =to_check.find(n->ptrs[1]->display);
        if (aux1!=to_check.end()){
            left=(this->*(aux1->second))(n->ptrs[0]);
        }
        if (aux2!=to_check.end()){
            right=(this->*(aux2->second))(n->ptrs[1]);
        }
        if (left.type=="BTYPE" && right.type=="BTYPE"){
            return Type("BTYPE");
        }
        else {
            TYPE_ERROR(n);
            return Type("BTYPE");
        }
    }
    Type RETURN(Node* n){
        auto aux =to_check.find(n->ptrs[0]->display);
        if (aux!=to_check.end()){
            if (to_return==(this->*(aux->second))(n->ptrs[0])){
                return to_return;
            }
            else {
                TYPE_ERROR(n);
                return to_return;
            }
        }
        else {
            TYPE_ERROR(n);
            return to_return;
        }
    }
    Type FUNCTIONCALL(Node* n){

        auto it = fun_params.find(n->ptrs[0]->ptrs[0]->display);
        //std::cout<<"EN FUNCALL\n";
        int c = n->ptrs[1]->ptrs.size();
        
        if (it!=fun_params.end()){
            if (c!=it->second.size()) {
                std::ostringstream oss;
                oss << "Invalid number of arguments in functioncall " << n->ptrs[0]->ptrs[0]->display << " at "<<" (" << n->row << "," << n->col << ")";
                std::string errorMessage = oss.str();
                errors.push_back(errorMessage);
                return Type("BTYPE");
            }
            //std::cout<<"EN FUNCALL\n";
            for (int i=0;i<it->second.size();i++){
                auto aux =to_check.find(n->ptrs[1]->ptrs[i]->display);
                if (aux!=to_check.end()){
                    if (it->second[i]!=(this->*(aux->second))(n->ptrs[1]->ptrs[i])){
                        TYPE_ERROR(n);
                        return Type("BTYPE");
                    }
                    //std::cout<<"EN FUNCALL SIN ERROR\n";
                }
                else {
                    ID_ERROR(n);
                    return Type("BTYPE");
                }
            }
            
            for (int i=ids.size()-1;i>=0;i--){
                auto it2 = ids[i].find(n->ptrs[0]->ptrs[0]->display);
                if (it2!=ids[i].end()){
                    return it2->second;
                }
            }
            ID_ERROR(n);
            return Type("BTYPE");
        }
        else {
            ID_ERROR(n);
            return Type("BTYPE");
        }
    }

    Type ID(Node* n){
        for (int i=ids.size()-1;i>=0;i--){
            auto it = ids[i].find(n->ptrs[0]->display);
            if (it!=ids[i].end()){
                return it->second;
            }
        }
        ID_ERROR(n);
        return Type("BTYPE");

    }
    Type OR(Node* n){
        Type left,right;
        auto aux1 =to_check.find(n->ptrs[0]->display);
        auto aux2 =to_check.find(n->ptrs[1]->display);
        if (aux1!=to_check.end()){
            left=(this->*(aux1->second))(n->ptrs[0]);
        }
        if (aux2!=to_check.end()){
            right=(this->*(aux2->second))(n->ptrs[1]);
        }
        if (left.type=="BTYPE" && right.type=="BTYPE"){
            return Type("BTYPE");
        }
        else {
            TYPE_ERROR(n);
            return Type("BTYPE");
        }
    }
    Type VARDECL(Node* n){
        //std::cout<<"Estoy en VARDECL\n"<<"Tipo: "<<n->ptrs[0]->display<<"\n";

        Type t (n->ptrs[0]->display);
        
        if (t.type=="ARRAY") {
            t.ndim = stoi(n->ptrs[0]->ptrs[1]->ptrs[0]->display);
            t.type=n->ptrs[0]->ptrs[0]->display;
        }
        //std::cout<<n->ptrs[1]->ptrs[0]->display<<"\n";
        if (!(ids.back().emplace(n->ptrs[1]->ptrs[0]->display,t).second)){
            ID_ERROR(n);
            return Type("BTYPE");
            //std::cout<<"ERROR55\n";
        }
        if (t.is_array() || n->ptrs.size()<3) {
            return t;
        }
        //ASSIGN
        //std::cout<<n->ptrs.size()<<"\n";
        auto it1 = to_check.find(n->ptrs[2]->display);
        //std::cout<<n->ptrs[2]->display<<"\n";

        if (it1!=to_check.end()){
            auto aux =(this->*(it1->second))(n->ptrs[2]);
            //std::cout<<aux.type<<"\n";
            if (aux!=t){
                TYPE_ERROR(n);
                return Type("BTYPE");
                //std::cout<<"ERROR1\n";
            }
            else {
                //std::cout<<"NO ERROR\n";
                return Type("ITYPE");
            }
        }
        else{
            TYPE_ERROR(n);
            return Type("BTYPE");
        }

    }
    Type ACCESS(Node* n){
        int count =1;
        while (n->ptrs[0]->display=="ACCESS"){
            n=n->ptrs[0];
            count++;
        }
        for (int i=ids.size()-1;i>=0;i--){
            auto it = ids[i].find(n->ptrs[0]->ptrs[0]->display);
            if (it!=ids[i].end())
            {
                if (it->second.ndim==count)
                    return Type (it->second.type);
                TYPE_ERROR(n);
                return Type("BTYPE");
            }
        }
        ID_ERROR(n);
        return Type("BTYPE");
    }
    Type NOT(Node* n){
        //std::cout<<"EN NOT, soy nodo "<<n->display<<"\n";
        Type t;
        auto aux1 =to_check.find(n->ptrs[0]->display);
        if (aux1!=to_check.end()){
            t=(this->*(aux1->second))(n->ptrs[0]);
        }
        if (t.type == "BTYPE" && !t.is_array()){
            return t;
        }
        else {
            TYPE_ERROR(n);
            return Type("BTYPE");
        }
    }
    Type NEG(Node* n){
        Type t;
        //std::cout<<"EN NODO NEG\n";
        //std::cout<<n->ptrs.size()<<"\n";
        //std::cout<<n->ptrs[0]->display<<"\n";
        auto aux1 =to_check.find(n->ptrs[0]->display);
        //std::cout<<aux1->first<<"\n";
        if (aux1!=to_check.end()){
            t=(this->*(aux1->second))(n->ptrs[0]);
        }
        else {
            TYPE_ERROR(n);
            return Type("ITYPE");
        }
        //std::cout<<t.type<<"\n";
        if (t.type!="ITYPE" || t.is_array()){
            TYPE_ERROR(n);
            return Type("ITYPE");
        }
        else {
            //std::cout<<"RETORNANDO DE NEG\n";
            return t;
        }
    }
    Type assign(Node* n){
        if (n->ptrs[0]->display=="ID"){
            auto it1 = to_check.find(n->ptrs[0]->display);
            auto it2 = to_check.find(n->ptrs[1]->display);
            if (it1!=to_check.end() && it2!=to_check.end()){
                if ((this->*(it1->second))(n->ptrs[0])!=(this->*(it2->second))(n->ptrs[1])){
                    TYPE_ERROR(n);
                    return Type("ITYPE");
                }
                else {
                    return Type("ITYPE");
                }
            }
        }
        else if (n->ptrs[0]->display=="ACCESS"){
            auto it1 = to_check.find(n->ptrs[0]->display);
            auto it2 = to_check.find(n->ptrs[1]->display);
            if (it1!=to_check.end() && it2!=to_check.end()){
                if ((this->*(it1->second))(n->ptrs[0]).type!=(this->*(it2->second))(n->ptrs[1]).type){
                    TYPE_ERROR(n);
                    return Type("ITYPE");
                }
                else {
                    return Type("ITYPE");
                }
            }
            else {
                TYPE_ERROR(n);
                return Type("BTYPE");
            }
        }
        else {
            TYPE_ERROR(n);
            return Type("ITYPE");
        }
        TYPE_ERROR(n);
        return Type("BTYPE");
    }
    Type addition(Node* n){
        auto left = to_check.find(n->ptrs[0]->display);
        auto right = to_check.find(n->ptrs[1]->display);
        Type t("ITYPE");
        if (left!=to_check.end() && right!=to_check.end()){
            auto aux1 = (this->*(left->second))(n->ptrs[0]);
            auto aux2 = (this->*(right->second))(n->ptrs[1]);
            if (aux1!=t || aux2!=t){
                    TYPE_ERROR(n);
                    return Type("ITYPE");
            }
            else {
                return Type("ITYPE");;
            }
        }
        else {
            TYPE_ERROR(n);
            return Type("ITYPE");
        }
    }
    Type subtraction(Node* n){
        auto left = to_check.find(n->ptrs[0]->display);
        auto right = to_check.find(n->ptrs[1]->display);
        Type t("ITYPE");
        if (left!=to_check.end() && right!=to_check.end()){
            auto aux1 = (this->*(left->second))(n->ptrs[0]);
            auto aux2 = (this->*(right->second))(n->ptrs[1]);
            if (aux1!=t || aux2!=t){
                    TYPE_ERROR(n);
                    return Type("ITYPE");
            }
            else {
                return Type("ITYPE");;
            }
        }
        else {
            TYPE_ERROR(n);
            return Type("ITYPE");
        }
    }
    Type multiplication(Node* n){
        auto left = to_check.find(n->ptrs[0]->display);
        auto right = to_check.find(n->ptrs[1]->display);
        Type t("ITYPE");
        if (left!=to_check.end() && right!=to_check.end()){
            auto aux1 = (this->*(left->second))(n->ptrs[0]);
            auto aux2 = (this->*(right->second))(n->ptrs[1]);
            if (aux1!=t || aux2!=t){
                    TYPE_ERROR(n);
                    return Type("ITYPE");
            }
            else {
                return Type("ITYPE");;
            }
        }
        else {
            TYPE_ERROR(n);
            return Type("ITYPE");
        }
    }
    Type division(Node* n){
        auto left = to_check.find(n->ptrs[0]->display);
        auto right = to_check.find(n->ptrs[1]->display);
        Type t("ITYPE");
        if (left!=to_check.end() && right!=to_check.end()){
            auto aux1 = (this->*(left->second))(n->ptrs[0]);
            auto aux2 = (this->*(right->second))(n->ptrs[1]);
            if (aux1!=t || aux2!=t){
                    TYPE_ERROR(n);
                    return Type("ITYPE");
            }
            else {
                return Type("ITYPE");;
            }
        }
        else {
            TYPE_ERROR(n);
            return Type("ITYPE");
        }
    }
    Type greater_than(Node* n){
        Type left,right;
        auto aux1 =to_check.find(n->ptrs[0]->display);
        auto aux2 =to_check.find(n->ptrs[1]->display);
        if (aux1!=to_check.end()){
            left=(this->*(aux1->second))(n->ptrs[0]);
        }
        if (aux2!=to_check.end()){
            right=(this->*(aux2->second))(n->ptrs[1]);
        }
        if (left.type==right.type && left.ndim == right.ndim && left.type!="STYPE" && left.type!="BTYPE"){
            return Type("BTYPE");
        }
        else {
            TYPE_ERROR(n);
            return Type("BTYPE");
        }
    }
    Type less_than(Node* n){
        Type left,right;
        auto aux1 =to_check.find(n->ptrs[0]->display);
        auto aux2 =to_check.find(n->ptrs[1]->display);
        if (aux1!=to_check.end()){
            left=(this->*(aux1->second))(n->ptrs[0]);
        }
        if (aux2!=to_check.end()){
            right=(this->*(aux2->second))(n->ptrs[1]);
        }
        if (left.type==right.type && left.ndim == right.ndim && left.type!="STYPE" && left.type!="BTYPE"){
            return Type("BTYPE");
        }
        else {
            TYPE_ERROR(n);
            return Type("BTYPE");
        }
    }
    Type equal(Node* n){
        Type left,right;
        auto aux1 =to_check.find(n->ptrs[0]->display);
        auto aux2 =to_check.find(n->ptrs[1]->display);
        if (aux1!=to_check.end()){
            left=(this->*(aux1->second))(n->ptrs[0]);
        }
        if (aux2!=to_check.end()){
            right=(this->*(aux2->second))(n->ptrs[1]);
        }
        if (left.type==right.type && left.ndim == right.ndim){
            return Type("BTYPE");
        }
        else {
            TYPE_ERROR(n);
            return Type("BTYPE");
        }
    }
    Type not_equal(Node* n){
        Type left,right;
        auto aux1 =to_check.find(n->ptrs[0]->display);
        auto aux2 =to_check.find(n->ptrs[1]->display);
        if (aux1!=to_check.end()){
            left=(this->*(aux1->second))(n->ptrs[0]);
        }
        if (aux2!=to_check.end()){
            right=(this->*(aux2->second))(n->ptrs[1]);
        }
        if (left.type==right.type && left.ndim == right.ndim){
            return Type("BTYPE");
        }
        else {
            TYPE_ERROR(n);
            return Type("BTYPE");
        }
    }
    Type greater_equal(Node* n){
        Type left,right;
        auto aux1 =to_check.find(n->ptrs[0]->display);
        auto aux2 =to_check.find(n->ptrs[1]->display);
        if (aux1!=to_check.end()){
            left=(this->*(aux1->second))(n->ptrs[0]);
        }
        if (aux2!=to_check.end()){
            right=(this->*(aux2->second))(n->ptrs[1]);
        }
        if (left.type==right.type && left.ndim == right.ndim && left.type!="STYPE" && left.type!="BTYPE"){
            return Type("BTYPE");
        }
        else {
            TYPE_ERROR(n);
            return Type("BTYPE");
        }
    }
    Type less_equal(Node* n){
        Type left,right;
        auto aux1 =to_check.find(n->ptrs[0]->display);
        auto aux2 =to_check.find(n->ptrs[1]->display);
        if (aux1!=to_check.end()){
            left=(this->*(aux1->second))(n->ptrs[0]);
        }
        if (aux2!=to_check.end()){
            right=(this->*(aux2->second))(n->ptrs[1]);
        }
        if (left.type==right.type && left.ndim == right.ndim && left.type!="STYPE" && left.type!="BTYPE"){
            return Type("BTYPE");
        }
        else {
            TYPE_ERROR(n);
            return Type("BTYPE");
        }
    }
    
    Type modulo(Node* n){
        auto left = to_check.find(n->ptrs[0]->display);
        auto right = to_check.find(n->ptrs[1]->display);
        Type t("ITYPE");
        if (left!=to_check.end() && right!=to_check.end()){
            auto aux1 = (this->*(left->second))(n->ptrs[0]);
            auto aux2 = (this->*(right->second))(n->ptrs[1]);
            if (aux1!=t || aux2!=t){
                    TYPE_ERROR(n);
                    return Type("ITYPE");
            }
            else {
                return Type("ITYPE");;
            }
        }
        else {
            TYPE_ERROR(n);
            return Type("ITYPE");
        }
    }

    
    TO_CHECK(){
        to_check.emplace("VARDECL",&TO_CHECK::VARDECL);
        to_check.emplace("+",&TO_CHECK::addition);
        to_check.emplace("-",&TO_CHECK::subtraction);
        to_check.emplace("*",&TO_CHECK::multiplication);
        to_check.emplace("/",&TO_CHECK::division);
        to_check.emplace("%",&TO_CHECK::modulo);
        to_check.emplace("NOT",&TO_CHECK::NOT);
        to_check.emplace("NEG",&TO_CHECK::NEG);
        to_check.emplace("=",&TO_CHECK::assign);
        to_check.emplace(">=",&TO_CHECK::greater_equal);
        to_check.emplace(">",&TO_CHECK::greater_than);
        to_check.emplace("<=",&TO_CHECK::less_equal);
        to_check.emplace("<",&TO_CHECK::less_than);
        to_check.emplace("==",&TO_CHECK::equal);
        to_check.emplace("!=",&TO_CHECK::not_equal);
        to_check.emplace("ACCESS",&TO_CHECK::ACCESS);
        to_check.emplace("AND",&TO_CHECK::AND);
        to_check.emplace("OR",&TO_CHECK::OR);
        to_check.emplace("FUNCTION",&TO_CHECK::FUNCTION);
        to_check.emplace("STMTLIST",&TO_CHECK::STMTLIST);
        to_check.emplace("PROGRAM",&TO_CHECK::PROGRAM);
        to_check.emplace("RETURN",&TO_CHECK::RETURN);
        to_check.emplace("ID",&TO_CHECK::ID);
        to_check.emplace("FUNCTIONCALL",&TO_CHECK::FUNCTIONCALL);
        to_check.emplace("ILITERAL",&TO_CHECK::ILITERAL);
        to_check.emplace("CLITERAL",&TO_CHECK::CLITERAL);
        to_check.emplace("BLITERAL",&TO_CHECK::BLITERAL);
        to_check.emplace("SLITERAL",&TO_CHECK::SLITERAL);
        to_check.emplace("IF",&TO_CHECK::IF);

    }
};



inline void rec_nodeTag(Node* n,std::ofstream& file, std::string &s){
    // if (n->ptrs.empty()) {
    //     std::cout<<"Vacio\n";
    //     return;
    // }
    if (!n) return;
    if (s.back()=='Z'){
        s.push_back('A');
    }
    else{
        s.back()++;
    }
    //std::cout<<"AQUI"<<s<<"\n";
    n->nodetag=s;
    if (n->display.back()=='\"'){
        file << "    "<<s<<" [label="<<n->display<<"];\n";
    }
    else{
        file << "    "<<s<<" [label="<<"\""<<n->display<<"\"];\n";
    }
    
    for (int i=0;i<n->ptrs.size();i++){
        if (!(n->ptrs[i])) continue;
        rec_nodeTag(n->ptrs[i],file,s);
    }
}

inline void rec_createDOT(Node* n,std::ofstream& file){
    // if (n->ptrs.empty()) {
    //     std::cout<<"Vacio\n";
    //     return;
    // }
    if (!n) return;
    for (int i=0;i<n->ptrs.size();i++){
        if (!(n->ptrs[i])) continue;
        file << "    "<<n->nodetag<<" -> "<<n->ptrs[i]->nodetag<<";\n";
    }
    for (int i=0;i<n->ptrs.size();i++){
        if (!(n->ptrs[i])) continue;
        rec_createDOT(n->ptrs[i],file);
    }
}

inline void createDOT(Node* root){
    std::ofstream file("graph.dot");
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        //return 1;
    }
    std::string s;
    s.push_back('A'-1);
    // Write the DOT graph structure
    file << "digraph G {\n";
    rec_nodeTag(root,file,s);
    rec_createDOT(root,file);
    file << "}\n";
    // Close the file stream
    file.close();
}


// struct NodeVisitor;


// struct Node{
//     virtual void accept(NodeVisitor &visitor)=0;
// };

// struct LiteralNode : Node {
//     std::variant<int,char,std::string,bool> value;
//     explicit LiteralNode (int val) : value(val){}
//     explicit LiteralNode (char val) : value(val){}
//     explicit LiteralNode (std::string val) : value(val){}
//     explicit LiteralNode (bool val) : value(val){}
//     void accept (NodeVisitor & visitor) override{
//         visitor.visit(*this);
//     }
// };

// struct BinaryOpNode : Node {
//     Token op;
//     Node* left;
//     Node* right;
//     BinaryOpNode(Token o, Node* l, Node* r) : op(o), left(l), right(r){}
//     void accept (NodeVisitor & visitor) override{
//         visitor.visit(*this);
//     }
// };

// struct UnaryOpNode : Node {
//     Token op;
//     Node* down;
//     UnaryOpNode(Token o, Node* d) : op(o), down(d){}
//     void accept (NodeVisitor & visitor) override{
//         visitor.visit(*this);
//     }
// };

// struct VariableNode : Node {
//     std::string name;
//     VariableNode (std::string &n) : name(n){}
//     void accept (NodeVisitor & visitor) override{
//         visitor.visit(*this);
//     }
// };

// struct VariableDeclaration : Node {
//     Token type;
//     Node* name;
//     VariableDeclaration (Token t, Node* n) : type(t), name(n){}
//     void accept (NodeVisitor & visitor) override{
//         visitor.visit(*this);
//     }
// };

// struct ForNode : Node {
//     Node* initialization;
//     Node* condition;
//     Node* increment;
//     Node* body;
//     ForNode(Node* ini, Node* con, Node* inc, Node* bod) : initialization(ini), condition(con), increment(inc), body(bod){}
//     void accept (NodeVisitor & visitor) override{
//         visitor.visit(*this);
//     }
// };

// struct WhileNode : Node {
//     Node* condition;
//     Node* body;
//     WhileNode(Node* c,Node*b) : condition(c), body(b){}
//     void accept (NodeVisitor & visitor) override{
//         visitor.visit(*this);
//     }
// };

// struct IfNode : Node {
//     Node* condition;
//     Node* body1;
//     Node* body2;
//     IfNode(Node* c, Node* b1, Node* b2) : condition(c), body1(b1), body2(b2){}
//     void accept (NodeVisitor & visitor) override{
//         visitor.visit(*this);
//     }
// };

// struct FunctionNode : Node {
//     std::string name;
//     Token return_type;
//     std::vector<Node*> arguments;
//     FunctionNode(std::string &n, Token t, std::vector<Node*> args) : name(n), return_type(t), arguments(args){}
//     void accept (NodeVisitor & visitor) override{
//         visitor.visit(*this);
//     }
// };

// struct NodeVisitor{
//     virtual void visit(LiteralNode &node) =0;
//     virtual void visit(BinaryOpNode &node) =0;
//     virtual void visit(UnaryOpNode &node) =0;
//     virtual void visit(VariableNode &node) =0;
//     virtual void visit(VariableDeclaration &node) =0;
//     virtual void visit(ForNode &node) =0;
//     virtual void visit(WhileNode &node) =0;
//     virtual void visit(IfNode &node) =0;
//     virtual void visit(FunctionNode &node) =0;
// };

// struct Printer : NodeVisitor{
//     void visit(LiteralNode &node) override {
//         std::cout<<"Literal(";
//         std::visit([](auto&& arg) {
//             std::cout << arg;
//         }, node.value);
//         std::cout << ")";
//     }
//     void visit(BinaryOpNode &node) override {

//     }
//     void visit(UnaryOpNode &node) override {

//     }
//     void visit(VariableNode &node) override {

//     }
//     void visit(VariableDeclaration &node) override {

//     }
//     void visit(ForNode &node) override {

//     }
//     void visit(WhileNode &node) override {

//     }
//     void visit(IfNode &node) override {

//     }
//     void visit(FunctionNode &node) override {

//     }
// };







inline std::string getTokenName(Token token) {
        switch (token) {
            case BRACKET_OPEN:       return "BRACKET_OPEN";
            case BRACKET_CLOSE:      return "BRACKET_CLOSE";
            //case INC:                return "INC";
            case PARENTHESIS_OPEN:   return "PARENTHESIS_OPEN";
            case PARENTHESIS_CLOSE:  return "PARENTHESIS_CLOSE";
            //case DEC:                return "DEC";
            case MINUS:              return "MINUS";
            case PLUS:               return "PLUS";
            case NOT:                return "NOT";
            case EXPONENT:           return "EXPONENT";
            case MULTIPLY:           return "MULTIPLY";
            case DIVIDE:             return "DIVIDE";
            case MODULO:             return "MODULO";
            case LESS_THAN:          return "LESS_THAN";
            case LESS_EQUAL:         return "LESS_EQUAL";
            case GREATER_THAN:       return "GREATER_THAN";
            case GREATER_EQUAL:      return "GREATER_EQUAL";
            case EQUAL:              return "EQUAL";
            case NOT_EQUAL:          return "NOT_EQUAL";
            case AND:                return "AND";
            case OR:                 return "OR";
            case ASSIGN:             return "ASSIGN";
            case COLON:              return "COLON";
            case BRACE_OPEN:         return "BRACE_OPEN";
            case BRACE_CLOSE:        return "BRACE_CLOSE";
            case COMMA:              return "COMMA";
            case SEMICOLON:          return "SEMICOLON";
            case STRING_VALUE:       return "STRING_VALUE";
            case CHAR_VALUE:         return "CHAR_VALUE";
            case NUMBER:             return "NUMBER";
            case IDENTIFIER:         return "IDENTIFIER";
            case INTEGER:            return "INTEGER";
            case BOOLEAN:            return "BOOLEAN";
            case CHAR:               return "CHAR";
            case STRING:             return "STRING";
            case FALSE:              return "FALSE";
            case TRUE:               return "TRUE";
            case ARRAY:              return "ARRAY";
            case ELSE:               return "ELSE";
            case FOR:                return "FOR";
            case FUNCTION:           return "FUNCTION";
            case IF:                 return "IF";
            case PRINT:              return "PRINT";
            case RETURN:             return "RETURN";
            case VOID:               return "VOID";
            case WHILE:              return "WHILE";
            case END_OF_FILE:        return "END_OF_FILE";
            default:                 return "UNKNOWN_TOKEN";
        }
    }

#endif