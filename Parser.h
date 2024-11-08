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
    bool parse(const std::vector<Token>& tokens,const std::vector<std::pair<int,int>>& position,const std::vector<std::string>& values);
};