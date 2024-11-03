#include "Parser.h"

void Parser::noMatch(const std::vector<Token>& expected)
{
    if (curr >= tokenPtr->size())
        logError();
    else
        logError(expected);
    throw 1;
}

void Parser::logError(const std::vector<Token>& expected)
{
    std::cout << "ERROR PARSER: Found " << (*tokenPtr)[curr] << ", expected: ";
    for (int i = 0; i < expected.size(); i++)
    {
        std::cout << expected[i] << ((i == expected.size()-1) ? '\n' : ' ');
    }
}

void Parser::logError()
{
    std::cout << "ERROR PARSER: No more tokens\n";
}

void Parser::synchronize(const std::vector<Token> &follow)
{
    errors++;
    std::cout << "DEBUG PARSE - synching: ";
    for (auto tok : follow)
        std::cout << tok << ' ';
    std::cout << '\n';
    auto it = std::find(follow.begin(), follow.end(), (*tokenPtr)[curr]);
    while (curr < (*tokenPtr).size() && it == follow.end())
    {
        it = std::find(follow.begin(), follow.end(), (*tokenPtr)[curr]);
        curr++;
    }
    if (curr >= tokenPtr->size())
        throw 'a';
}

bool Parser::check(const std::vector<Token> &tokList)
{   
    return curr < (*tokenPtr).size() && std::find(tokList.begin(), tokList.end(), (*tokenPtr)[curr]) != tokList.end();
}

bool Parser::terminal(Token token)
{
    if (curr >= tokenPtr->size())
    {
        logError();
        throw 1;
    }
    if ((*tokenPtr)[curr] != token)
    {
        logError({token});
        throw 1;
    }
    curr++;
    return true;
}

bool Parser::program()
{
    try
    {
        if (check({INTEGER, BOOLEAN, CHAR, STRING, VOID}))
            return declaration() && programPrime();
        noMatch({INTEGER, BOOLEAN, CHAR, STRING, VOID});
    }
    catch (int err)
    {
        synchronize({END_OF_FILE});
    }
    return true;
}

bool Parser::programPrime()
{
    try
    {
        if (check({INTEGER, BOOLEAN, CHAR, STRING, VOID}))
            return declaration() && programPrime();
        if (check({END_OF_FILE}))
            return true;
        noMatch({INTEGER, BOOLEAN, CHAR, STRING, VOID, END_OF_FILE});
    }
    catch (int err)
    {
        synchronize({END_OF_FILE});
    }
    return true;
}

bool Parser::declaration()
{
    try
    {
        if (check({INTEGER, BOOLEAN, CHAR, STRING, VOID}))
            return type() && terminal(IDENTIFIER) && declarationPrime();
        noMatch({INTEGER, BOOLEAN, CHAR, STRING, VOID});
    }
    catch (int err)
    {
        synchronize({INTEGER, BOOLEAN, CHAR, STRING, VOID, END_OF_FILE});
    }
    return true;
}

bool Parser::declarationPrime()
{
    try
    {
        if (check({PARENTHESIS_OPEN}))
            return function();
        if (check({SEMICOLON, ASSIGN}))
            return varDecl();
        noMatch({PARENTHESIS_OPEN, SEMICOLON, ASSIGN});
    }
    catch (int err)
    {
        synchronize({INTEGER, BOOLEAN, CHAR, STRING, VOID, END_OF_FILE});
    }
    return true;
}

bool Parser::function()
{
    try
    {
        if (check({PARENTHESIS_OPEN}))
            return terminal(PARENTHESIS_OPEN) && params() && terminal(PARENTHESIS_CLOSE) && terminal(BRACE_OPEN) && stmtList() && terminal(BRACE_CLOSE);
        noMatch({PARENTHESIS_OPEN});
    }
    catch (int err)
    {
        synchronize({INTEGER, BOOLEAN, CHAR, STRING, VOID, END_OF_FILE});
    }
    return true;
}

bool Parser::type()
{
    try
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
        noMatch({INTEGER, BOOLEAN, CHAR, STRING, VOID});
    }
    catch (int err)
    {
        synchronize({IDENTIFIER});
    }
    return true;
}

bool Parser::typePrime()
{
    try
    {
        if (check({BRACKET_OPEN}))
            return terminal(BRACKET_OPEN) && terminal(BRACKET_CLOSE) && typePrime();
        if (check({IDENTIFIER}))
            return true;
        noMatch({BRACKET_OPEN, IDENTIFIER});
    }
    catch (int err)
    {
        synchronize({IDENTIFIER});
    }
    return true;
}

bool Parser::params()
{
    try
    {
        if (check({INTEGER, BOOLEAN, CHAR, STRING, VOID}))
            return type() && terminal(IDENTIFIER) && paramsPrime();
        if (check({PARENTHESIS_CLOSE}))
            return true;
        noMatch({INTEGER, BOOLEAN, CHAR, STRING, VOID, PARENTHESIS_CLOSE});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE});
    }
    return true;
}

bool Parser::paramsPrime()
{
    try
    {
        if (check({COMMA}))
            return terminal(COMMA) && params();
        if (check({PARENTHESIS_CLOSE}))
            return true;
        noMatch({COMMA, PARENTHESIS_CLOSE});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE});
    }
    return true;
}

bool Parser::varDecl()
{
    try
    {
        if (check({SEMICOLON}))
            return terminal(SEMICOLON);
        if (check({ASSIGN}))
            return terminal(ASSIGN) && expression() && terminal(SEMICOLON);
        noMatch({SEMICOLON, ASSIGN});
    }
    catch (int err)
    {
        synchronize({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, BRACE_CLOSE, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE, END_OF_FILE});
    }
    return true;
}

bool Parser::stmtList()
{
    try
    {
        if (check({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
            return statement() && stmtListPrime();
        noMatch({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    catch (int err)
    {
        synchronize({BRACE_CLOSE});
    }
    return true;
}

bool Parser::stmtListPrime()
{
    try
    {
        if (check({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
            return statement() && stmtListPrime();
        if (check({BRACE_CLOSE}))
            return true;
        noMatch({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE, BRACE_CLOSE});
    }
    catch (int err)
    {
        synchronize({BRACE_CLOSE});
    }
    return true;
}

bool Parser::statement()
{
    try
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
        noMatch({INTEGER, BOOLEAN, CHAR, STRING, VOID, IF, FOR, RETURN, IDENTIFIER, PARENTHESIS_OPEN, SEMICOLON, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE, PRINT, BRACE_OPEN});
    }
    catch (int err)
    {
        synchronize({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, BRACE_CLOSE, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    return true;
}

bool Parser::ifStmt()
{
    try
    {
        if (check({IF}))
            return terminal(IF) && terminal(PARENTHESIS_OPEN) && expression() && terminal(PARENTHESIS_CLOSE) && terminal(BRACE_OPEN) && stmtList() && terminal(BRACE_CLOSE) && ifStmtPrime();
        noMatch({IF});
    }
    catch (int err)
    {
        synchronize({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, BRACE_CLOSE, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    return true;
}

bool Parser::ifStmtPrime()
{
    try
    {
        if (check({ELSE}))
            terminal(ELSE) && terminal(BRACE_OPEN) && stmtList() && terminal(BRACE_CLOSE);
        if (check({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, BRACE_CLOSE, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
            return true;
        noMatch({ELSE, IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, BRACE_CLOSE, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    catch (int err)
    {
        synchronize({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, BRACE_CLOSE, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    return true;
}

bool Parser::forStmt()
{
    try
    {
        if (check({FOR}))
            return terminal(FOR) && terminal(PARENTHESIS_OPEN) && exprStmt() && expression() && terminal(SEMICOLON) && exprStmt() && terminal(PARENTHESIS_CLOSE) && statement();
        noMatch({FOR});
    }
    catch (int err)
    {
        synchronize({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, BRACE_CLOSE, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    return true;
}

bool Parser::returnStmt()
{
    try
    {
        if (check({RETURN}))
            return terminal(RETURN) && expression() && terminal(SEMICOLON);
        noMatch({RETURN});
    }
    catch (int err)
    {
        synchronize({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, BRACE_CLOSE, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    return true;
}

bool Parser::printStmt()
{
    try 
    {
        if (check({PRINT}))
            return terminal(PRINT) && terminal(PARENTHESIS_OPEN) && exprList() && terminal(PARENTHESIS_CLOSE) && terminal(SEMICOLON);
        noMatch({PRINT});
    }
    catch (int err)
    {
        synchronize({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, BRACE_CLOSE, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    return true;
}

bool Parser::exprStmt()
{
    try
    {
        if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
            return expression() && terminal(SEMICOLON);
        if (check({SEMICOLON}));
            return terminal(SEMICOLON);
        noMatch({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE, SEMICOLON});
    }
    catch (int err)
    {
        synchronize({IDENTIFIER, PARENTHESIS_OPEN, PARENTHESIS_CLOSE, BRACE_OPEN, BRACE_CLOSE, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    return true;
}

bool Parser::exprList()
{
    try
    {
        if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
            return expression() && exprListPrime();
        noMatch({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE});
    }
    return true;
}

bool Parser::exprListPrime()
{
    try
    {
        if (check({COMMA}))
            return terminal(COMMA) && exprList();
        if (check({PARENTHESIS_CLOSE}))
            return true;
        noMatch({COMMA, PARENTHESIS_CLOSE});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE});        
    }
    return true;
}

bool Parser::expression()
{
    try
    {
        if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
            return orExpr() && expressionPrime();
        noMatch({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON});
    }
    return true;
}

bool Parser::expressionPrime()
{
    try
    {
        if (check({ASSIGN}))
            return terminal(ASSIGN) && orExpr();
        if (check({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON}))
            return true;
        noMatch({ASSIGN, PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON});
    }
    return true;
}

bool Parser::orExpr()
{
    try
    {
        if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
            return andExpr() && orExprPrime();
        noMatch({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN});
    }
    return true;
}

bool Parser::orExprPrime()
{
    try
    {
        if (check({OR}))
            return terminal(OR) && andExpr() && orExprPrime();
        if (check({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN}))
            return true;
        noMatch({OR, PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN});
    }
    return true;
}

bool Parser::andExpr()
{
    try
    {
        if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
            return eqExpr() && andExprPrime();
        noMatch({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR});
    }
    return true;
}

bool Parser::andExprPrime()
{
    try
    {
        if (check({AND}))
            return terminal(AND) && eqExpr() && andExprPrime();
        if (check({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR}))
            return true;
        noMatch({AND, PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR});
    }
    return true;
}

bool Parser::eqExpr()
{
    try
    {
        if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
            return relExpr() && eqExprPrime();
        noMatch({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND});
    }
    return true;
}

bool Parser::eqExprPrime()
{
    try
    {
        if (check({EQUAL}))
            return terminal(EQUAL) && relExpr() && eqExprPrime();
        if (check({NOT_EQUAL}))
            return terminal(NOT_EQUAL) && relExpr() && eqExprPrime();
        if (check({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND}))
            return true;
        noMatch({EQUAL, NOT_EQUAL, PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND});
    }
    return true;
}

bool Parser::relExpr()
{
    try
    {
        if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
            return expr() && relExprPrime();
        noMatch({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL});
    }
    return true;
}

bool Parser::relExprPrime()
{
    try
    {
        if (check({LESS_THAN}))
            return terminal(LESS_THAN) && expr() && relExprPrime();
        if (check({GREATER_THAN}))
            return terminal(GREATER_THAN) && expr() && relExprPrime();
        if (check({LESS_EQUAL}))
            return terminal(LESS_EQUAL) && expr() && relExprPrime();
        if (check({GREATER_EQUAL}))
            return terminal(GREATER_EQUAL) && expr() && relExprPrime();
        if (check({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL}))
            return true;
        noMatch({LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL});
    }
    return true;
}

bool Parser::expr()
{
    try
    {
        if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
            return term() && exprPrime();
        noMatch({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL});
    }
    return true;
}

bool Parser::exprPrime()
{
    try
    {
        if (check({PLUS}))
            return terminal(PLUS) && term() && exprPrime();
        if (check({MINUS}))
            return terminal(MINUS) && term() && exprPrime();
        if (check({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL}))
            return true;
        noMatch({PLUS, MINUS, PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL});
    }
    return true;
}

bool Parser::term()
{
    try
    {
        if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
            return unary() && termPrime();
        noMatch({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS});
    }
    return true;
}

bool Parser::termPrime()
{
    try
    {
        if (check({MULTIPLY}))
            return terminal(MULTIPLY) && unary() && termPrime();
        if (check({DIVIDE}))
            return terminal(DIVIDE) && unary() && termPrime();
        if (check({MODULO}))
            return terminal(MODULO) && unary() && termPrime();
        if (check({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS}))
            return true;
        noMatch({MULTIPLY, DIVIDE, MODULO, PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS});
    }
    return true;
}

bool Parser::unary()
{
    try
    {
        if (check({NOT}))
            return terminal(NOT) && unary();
        if (check({MINUS}))
            return terminal(MINUS) && unary();
        if (check({IDENTIFIER, PARENTHESIS_OPEN, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
            return factor();
        noMatch({NOT, MINUS, IDENTIFIER, PARENTHESIS_OPEN, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS, MULTIPLY, DIVIDE, MODULO});
    }
    return true;
}

bool Parser::factor()
{
    try
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
        noMatch({IDENTIFIER, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE, PARENTHESIS_OPEN});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS, MULTIPLY, DIVIDE, MODULO});
    }
    return true;
}

bool Parser::factorPrime1()
{
    try
    {
        if (check({PARENTHESIS_OPEN}))
            return terminal(PARENTHESIS_OPEN) && exprList() && terminal(PARENTHESIS_CLOSE);
        if (check({PARENTHESIS_CLOSE, BRACKET_OPEN, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS, MULTIPLY, DIVIDE, MODULO}))
            return true;
        noMatch({PARENTHESIS_OPEN, PARENTHESIS_CLOSE, BRACKET_OPEN, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS, MULTIPLY, DIVIDE, MODULO});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_OPEN, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS, MULTIPLY, DIVIDE, MODULO});
    }
    return true;
}

bool Parser::factorPrime2()
{
    try
    {
        if (check({BRACKET_OPEN}))
            return terminal(BRACKET_OPEN) && expression() && terminal(BRACKET_CLOSE) && factorPrime2();
        if (check({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS, MULTIPLY, DIVIDE, MODULO}))
            return true;
        noMatch({BRACKET_OPEN, PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS, MULTIPLY, DIVIDE, MODULO});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS, MULTIPLY, DIVIDE, MODULO});
    }
    return true;
}

bool Parser::parse(const std::vector<Token>& tokens)
{
    curr = 0;
    tokenPtr = &tokens;
    errors = 0;
    try
    {
        program();
    }
    catch(char c)
    {
        std::cout << "ERROR PARSER: Sudden end of program\n";
    }

    std::cout << "INFO PARSE - Completed with " << errors << " errors.\n";

    return !errors;        
}
