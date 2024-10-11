#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "common.h"

class Parser
{
    const std::vector<Token>* tokenPtr = nullptr;
    int curr = 0;

    bool check(const std::vector<Token>& tokList);
    bool terminal(Token token);

    bool program();
    bool programPrime();
    bool declaration();
    bool declarationPrime();
    bool function();
    bool type();
    bool typePrime();
    bool params();
    bool paramsPrime();
    bool varDecl();
    bool stmtList();
    bool stmtListPrime();
    bool statement();
    bool ifStmt();
    bool ifStmtPrime();
    bool forStmt();
    bool returnStmt();
    bool printStmt();
    bool exprStmt();
    bool exprList();
    bool exprListPrime();
    bool expression();
    bool expressionPrime();
    bool orExpr();
    bool orExprPrime();
    bool andExpr();
    bool andExprPrime();
    bool eqExpr();
    bool eqExprPrime();
    bool relExpr();
    bool relExprPrime();
    bool expr();
    bool exprPrime();
    bool term();
    bool termPrime();
    bool unary();
    bool factor();
    bool factorPrime1();
    bool factorPrime2();

public:
    bool parse(const std::vector<Token>& tokens);
};