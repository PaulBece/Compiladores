#ifndef COMMON_HEADER
#define COMMON_HEADER

#include <iostream>
#include <vector>
#include <string>
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

struct TO_CHECK{
    std::unordered_map<std::string,void(*)(Node*n)> to_check;
    std::unordered_map<std::string,std::string> ids;
    std::unordered_map<std::string,std::pair<std::string,int>> arrays;
    TO_CHECK(){
        to_check.emplace("VARDECL",&VARDECL);
        to_check.emplace("+",&addition);
        to_check.emplace("-",&substraction);
        to_check.emplace("*",&multiplication);
        to_check.emplace("/",&division);
        to_check.emplace("%",&modulo);
        to_check.emplace("NOT",&NOT);
        to_check.emplace("NEG",&NEG);
        to_check.emplace("=",&assign);
        to_check.emplace(">=",&greater_equal);
        to_check.emplace(">",&greater_than);
        to_check.emplace("<=",&less_equal);
        to_check.emplace("<",&less_than);
        to_check.emplace("==",&equal);
        to_check.emplace("!=",&not_equal);
        to_check.emplace("ACCESS",&ACCESS);
        to_check.emplace("AND",&AND);
        to_check.emplace("OR",&OR);
    }
    void id_types(Node* n){
        if (!n) return;
        if (n->display=="VARDECL"){
            if (n->ptrs[0]->display=="ARRAY")
            ids.emplace(n->ptrs[1]->ptrs[0]->display,n->ptrs[0]->display);
        }
        for (int i=0;i<n->ptrs.size();i++){
            id_types(n->ptrs[i]);
        }
    }


    void AND(Node* n){
    
    }
    void OR(Node* n){
    
    }
    void VARDECL(Node* n){
    
    }
    void ACCESS(Node* n){
    
    }
    void NOT(Node* n){
        
    }
    void NEG(Node* n){
        if (n->ptrs[0]->display!="ILITERAL"){
            //ERROR
        }
        if (n->ptrs[0]->display!="ILITERAL"){

        }
    }
    void assign(Node* n){
    
    }
    void addition(Node* n){
        
    }
    void substraction(Node* n){
    
    }
    void multiplication(Node* n){
    
    }
    void division(Node* n){
    
    }
    void greater_than(Node* n){
    
    }
    void less_than(Node* n){
    
    }
    void equal(Node* n){
    
    }
    void not_equal(Node* n){
    
    }
    void greater_equal(Node* n){
    
    }
    void less_equal(Node* n){
    
    }
    void modulo(Node* n){
    
    }
};

void check(Node* n, std::unordered_map<std::string,void(*)(Node*n)> &to_check){
    if (!n) return;
    auto aux =to_check.find(n->display);
    if (aux!=to_check.end()){
        aux->second(n);
    }
    for (int i=0;i<n->ptrs.size();i++){
        check(n->ptrs[i],to_check);
    }
}

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