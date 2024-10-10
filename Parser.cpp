#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "common.h"

std::vector<Token> tokens;

int curr = 0;

void error(std::string msg)
{
    std::cout << "Error: " << msg << '\n';
}

bool check(const std::vector<Token>& tokList)
{   
    return curr < tokens.size() && std::find(tokList.begin(), tokList.end(), tokens[curr]) != tokList.end();
}

bool terminal(Token token)
{
    return curr < tokens.size() && tokens[curr++] == token;
}

bool eval();
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

bool eval()
{

}

bool program()
{
    if (check({INTEGER, BOOLEAN, CHAR, STRING, VOID}))
        return declaration() && programPrime();
    return false;
}

bool programPrime()
{
    if (check({INTEGER, BOOLEAN, CHAR, STRING, VOID}))
        return declaration() && programPrime();
    if (check({END_OF_FILE}))
        return true;
    return false;
}

bool declaration()
{
    if (check({INTEGER, BOOLEAN, CHAR, STRING, VOID}))
        return type() && terminal(IDENTIFIER) && declarationPrime();
    return false;
}

bool declarationPrime()
{
    if (check({PARENTHESIS_OPEN}))
        return function();
    if (check({SEMICOLON, EQUAL}))
        return varDecl();
    return false;
}

bool function()
{
    if (check({PARENTHESIS_OPEN}))
        return terminal(PARENTHESIS_OPEN) && params() && terminal(PARENTHESIS_CLOSE) && terminal(BRACE_OPEN) && stmtList() && terminal(BRACE_CLOSE);
    return false;
}

bool type()
{
    if (check({INTEGER}))
        return terminal(INTEGER) && typePrime();
    if (check({BOOLEAN}))
        return terminal(BOOLEAN) && typePrime();
    if (check({CHAR}))
        return terminal(CHAR) && typePrime();
    if (check({STRING}))
        return terminal(STRING) && typePrime();
    if (check({VOID}))
        return terminal(VOID) && typePrime();
    return false;
}

bool typePrime()
{
    if (check({BRACKET_OPEN}))
        return terminal(BRACKET_OPEN) && terminal(BRACKET_CLOSE) && typePrime();
    if (check({IDENTIFIER}))
        return true;
    return false;
}

bool params()
{
    if (check({INTEGER, BOOLEAN, CHAR, STRING, VOID}))
        return type() && terminal(IDENTIFIER) && paramsPrime();
    if (check({PARENTHESIS_CLOSE}))
        return true;
    return false;
}

bool paramsPrime()
{
    if (check({COMMA}))
        return terminal(COMMA) && params();
    if (check({PARENTHESIS_CLOSE}))
        return true;
    return false;
}

bool varDecl()
{
    if (check({SEMICOLON}))
        return terminal(SEMICOLON);
    if (check({EQUAL}))
        return terminal(EQUAL) && expression() && terminal(SEMICOLON);
    return false;
}

bool stmtList()
{
    if (check({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
        return statement() && stmtListPrime();
    return false;
}

bool stmtListPrime()
{
    if (check({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
        return statement() && stmtListPrime();
    if (check({BRACE_CLOSE}))
        return true;
    return false;
}

bool statement()
{
    if (check({INTEGER, BOOLEAN, CHAR, STRING, VOID}))
        return type() && terminal(IDENTIFIER) && varDecl();
    if (check({IF}))
        return ifStmt();
    if (check({FOR}))
        return forStmt();
    if (check({RETURN}))
        return returnStmt();
    if (check({IDENTIFIER, PARENTHESIS_OPEN, SEMICOLON, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
        return exprStmt();
    if (check({PRINT}))
        return printStmt();
    if (check({BRACE_OPEN}))
        return terminal(BRACE_OPEN) && stmtList() && terminal(BRACE_CLOSE);
    return false;
}