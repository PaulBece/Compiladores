#include "common.h"

class Parser
{
    const std::vector<Token>* tokenPtr = nullptr;
    const std::vector<std::pair<int,int>>* positionPtr = nullptr;
    const std::vector<std::string>* valuesPtr = nullptr;
    int curr = 0;
    int errors = 0;

    void noMatch(const std::vector<Token>& expected);
    void logError(const std::vector<Token>& expected);
    void logError();

    void synchronize(const std::vector<Token>& follow);

    bool check(const std::vector<Token>& tokList);
    void terminal(Token token);
    void terminal(Token token, std::string& val);

    Node* program();
    Node* programPrime(std::vector<Node*>& prevVals);
    Node* declaration();
    Node* declarationPrime(Node* typeNode, Node* idNode);
    Node* function(Node* typeNode, Node* idNode);
    Node* type();
    Node* typePrime(Node* typeNode, int cnt = 0);
    Node* params();
    Node* paramsPrime(Node* firstVal);
    Node* varDecl(Node* typeNode, Node* idNode);
    Node* stmtList();
    Node* stmtListPrime(std::vector<Node*>& prevVals);
    Node* statement();
    Node* ifStmt();
    Node* ifStmtPrime();
    Node* forStmt();
    Node* returnStmt();
    Node* printStmt();
    Node* exprStmt();
    Node* exprList();
    Node* exprListPrime(Node* firstVal);
    Node* expression();
    Node* expressionPrime(Node* firstVal);
    Node* orExpr();
    Node* orExprPrime(Node* firstVal);
    Node* andExpr();
    Node* andExprPrime(Node* firstVal);
    Node* eqExpr();
    Node* eqExprPrime(Node* firstVal);
    Node* relExpr();
    Node* relExprPrime(Node* firstVal);
    Node* expr();
    Node* exprPrime(Node* firstVal);
    Node* term();
    Node* termPrime(Node* firstVal);
    Node* unary();
    Node* factor();
    Node* factorPrime1(Node* firstVal);
    Node* factorPrime2(Node* firstVal);

public:
    bool parse(const std::vector<Token>& tokens,const std::vector<std::pair<int,int>>& position,const std::vector<std::string>& values);
};