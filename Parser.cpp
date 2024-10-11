#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
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
    return false;
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
    if (check({SEMICOLON, ASSIGN}))
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
    if (check({ASSIGN}))
        return terminal(ASSIGN) && expression() && terminal(SEMICOLON);
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

bool ifStmt()
{
    if (check({IF}))
        return terminal(IF) && terminal(PARENTHESIS_OPEN) && expression() && terminal(PARENTHESIS_CLOSE) && terminal(BRACE_OPEN) && statement() && terminal(BRACE_CLOSE) && ifStmtPrime();
    return false;
}

bool ifStmtPrime()
{
    if (check({ELSE}))
        terminal(ELSE) && terminal(BRACE_OPEN) && statement() && terminal(BRACE_CLOSE);
    if (check({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, BRACE_CLOSE, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
        return true;
    return false;
}

bool forStmt()
{
    if (check({FOR}))
        return terminal(FOR) && terminal(PARENTHESIS_OPEN) && exprStmt() && expression() && terminal(SEMICOLON) && exprStmt() && terminal(PARENTHESIS_CLOSE) && statement();
    return false;
}

bool returnStmt()
{
    if (check({RETURN}))
        return terminal(RETURN) && expression() && terminal(SEMICOLON);
    return false;
}

bool printStmt()
{
    if (check({PRINT}))
        return terminal(PRINT) && terminal(PARENTHESIS_OPEN) && exprList() && terminal(PARENTHESIS_CLOSE) && terminal(SEMICOLON);
    return false;
}

bool exprStmt()
{
    if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
        return expression() && terminal(SEMICOLON);
    if (check({SEMICOLON}));
        return terminal(SEMICOLON);
    return false;
}

bool exprList()
{
    if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
        return expression() && exprListPrime();
    return false;
}

bool exprListPrime()
{
    if (check({COMMA}))
        return terminal(COMMA) && exprList();
    if (check({PARENTHESIS_CLOSE}))
        return true;
    return false;
}

bool expression()
{
    if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
        return orExpr() && expressionPrime();
    return false;
}

bool expressionPrime()
{
    if (check({ASSIGN}))
        return terminal(ASSIGN) && orExpr();
    if (check({PARENTHESIS_CLOSE, BRACKET_OPEN, COMMA, SEMICOLON}))
        return true;
    return false;
}

bool orExpr()
{
    if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
        return andExpr() && orExprPrime();
    return false;
}

bool orExprPrime()
{
    if (check({OR}))
        return terminal(OR) && andExpr() && orExprPrime();
    if (check({PARENTHESIS_CLOSE, BRACKET_OPEN, COMMA, SEMICOLON, ASSIGN}))
        return true;
    return false;
}

bool andExpr()
{
    if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
        return eqExpr() && andExprPrime();
    return false;
}

bool andExprPrime()
{
    if (check({AND}))
        return terminal(AND) && eqExpr() && andExprPrime();
    if (check({PARENTHESIS_CLOSE, BRACKET_OPEN, COMMA, SEMICOLON, ASSIGN, OR}))
        return true;
    return false;
}

bool eqExpr()
{
    if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
        return relExpr() && eqExprPrime();
    return false;
}

bool eqExprPrime()
{
    if (check({EQUAL}))
        return terminal(EQUAL) && relExpr() && eqExprPrime();
    if (check({NOT_EQUAL}))
        return terminal(NOT_EQUAL) && relExpr() && eqExprPrime();
    if (check({PARENTHESIS_CLOSE, BRACKET_OPEN, COMMA, SEMICOLON, ASSIGN, OR, AND}))
        return true;
    return false;
}

bool relExpr()
{
    if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
        return expr() && relExprPrime();
    return false;
}

bool relExprPrime()
{
    if (check({LESS_THAN}))
        return terminal(LESS_THAN) && expr() && relExprPrime();
    if (check({GREATER_THAN}))
        return terminal(GREATER_THAN) && expr() && relExprPrime();
    if (check({LESS_EQUAL}))
        return terminal(LESS_EQUAL) && expr() && relExprPrime();
    if (check({GREATER_EQUAL}))
        return terminal(GREATER_EQUAL) && expr() && relExprPrime();
    if (check({PARENTHESIS_CLOSE, BRACKET_OPEN, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL}))
        return true;
    return false;
}

bool expr()
{
    if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
        return term() && exprPrime();
    return false;
}

bool exprPrime()
{
    if (check({PLUS}))
        return terminal(PLUS) && term() && exprPrime();
    if (check({MINUS}))
        return terminal(MINUS) && term() && exprPrime();
    if (check({PARENTHESIS_CLOSE, BRACKET_OPEN, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL}))
        return true;
    return false;
}

bool term()
{
    if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
        return unary() && termPrime();
    return false;
}

bool termPrime()
{
    if (check({MULTIPLY}))
        return terminal(MULTIPLY) && unary() && termPrime();
    if (check({DIVIDE}))
        return terminal(DIVIDE) && unary() && termPrime();
    if (check({MODULO}))
        return terminal(MODULO) && unary() && termPrime();
    if (check({PARENTHESIS_CLOSE, BRACKET_OPEN, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS}))
        return true;
    return false;
}

bool unary()
{
    if (check({NOT}))
        return terminal(NOT) && unary();
    if (check({MINUS}))
        return terminal(MINUS) && unary();
    if (check({IDENTIFIER, PARENTHESIS_OPEN, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
        return factor();
    return false;
}

bool factor()
{
    if (check({IDENTIFIER}))
        return terminal(IDENTIFIER) && factorPrime1() && factorPrime2();
    if (check({NUMBER}))
        return terminal(NUMBER) && factorPrime2();
    if (check({CHAR_VALUE}))
        return terminal(CHAR_VALUE) && factorPrime2();
    if (check({STRING_VALUE}))
        return terminal(STRING_VALUE) && factorPrime2();
    if (check({TRUE}))
        return terminal(TRUE) && factorPrime2();
    if (check({FALSE}))
        return terminal(FALSE) && factorPrime2();
    if (check({PARENTHESIS_OPEN}))
        return terminal(PARENTHESIS_OPEN) && expression() && terminal(PARENTHESIS_CLOSE) && factorPrime2();
    return false;
}

bool factorPrime1()
{
    if (check({PARENTHESIS_OPEN}))
        return terminal(PARENTHESIS_OPEN) && exprList() && terminal(PARENTHESIS_CLOSE);
    if (check({PARENTHESIS_CLOSE, BRACKET_OPEN, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS, MULTIPLY, DIVIDE, MODULO}))
        return true;
    return false;
}

bool factorPrime2()
{
    if (check({BRACKET_OPEN}))
        return terminal(BRACE_OPEN) && expression() && terminal(BRACE_CLOSE) && factorPrime2();
    if (check({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS, MULTIPLY, DIVIDE, MODULO}))
        return true;
    return false;
}

int main()
{
    tokens = {INTEGER, IDENTIFIER, ASSIGN, NUMBER, SEMICOLON, NUMBER, PLUS, NUMBER, SEMICOLON, END_OF_FILE};
    std::cout << program() << '\n';
    tokens = {VOID, IDENTIFIER, PARENTHESIS_OPEN, INTEGER, IDENTIFIER, PARENTHESIS_CLOSE, BRACE_OPEN, NUMBER, ASSIGN, NUMBER, SEMICOLON, BRACE_CLOSE, END_OF_FILE};
    curr = 0;
    std::cout << program() << '\n';
}