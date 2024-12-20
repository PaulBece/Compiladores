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
    if ((*valuesPtr)[curr].size()!=0)
        std::cout << "ERROR PARSER: Found " << getTokenName((*tokenPtr)[curr])<<" at ("<<(*positionPtr)[curr].first<<","<<(*positionPtr)[curr].second<<")" <<" with value "<<(*valuesPtr)[curr]<< ", expected: ";
    else
        std::cout << "ERROR PARSER: Found " << getTokenName((*tokenPtr)[curr])<<" at ("<<(*positionPtr)[curr].first<<","<<(*positionPtr)[curr].second<<")" << ", expected: ";
    for (int i = 0; i < expected.size(); i++)
    {
        std::cout << getTokenName(expected[i]) << ((i == expected.size()-1) ? '\n' : ' ');
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
        std::cout << getTokenName(tok) << ' ';
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

void Parser::terminal(Token token)
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
}

void Parser::terminal(Token token, std::string& val)
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
    val = (*valuesPtr)[curr];
    curr++;
}

Node* Parser::program()
{
    try
    {
        if (check({INTEGER, BOOLEAN, CHAR, STRING, VOID}))
        {
            std::vector<Node*> vec = {declaration()};
            return programPrime(vec);
        }
        noMatch({INTEGER, BOOLEAN, CHAR, STRING, VOID});
    }
    catch (int err)
    {
        synchronize({END_OF_FILE});
    }
    return nullptr;
}

Node* Parser::programPrime(std::vector<Node*>& prevVals)
{
    try
    {
        if (check({INTEGER, BOOLEAN, CHAR, STRING, VOID}))
        {
            prevVals.push_back(declaration());
            return programPrime(prevVals);
        }
        if (check({END_OF_FILE}))
            return new Node("PROGRAM", prevVals, (*positionPtr)[curr].first, (*positionPtr)[curr].second);
        noMatch({INTEGER, BOOLEAN, CHAR, STRING, VOID, END_OF_FILE});
    }
    catch (int err)
    {
        synchronize({END_OF_FILE});
    }
    return nullptr;
}

Node* Parser::declaration()
{
    try
    {
        if (check({INTEGER, BOOLEAN, CHAR, STRING, VOID}))
        {
            std::string id;
            auto fp = type();
            return declarationPrime(fp, new Node("ID", {new Node((terminal(IDENTIFIER, id), id), {}, (*positionPtr)[curr].first, (*positionPtr)[curr].second)}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        }
        noMatch({INTEGER, BOOLEAN, CHAR, STRING, VOID});
    }
    catch (int err)
    {
        synchronize({INTEGER, BOOLEAN, CHAR, STRING, VOID, END_OF_FILE});
    }
    return nullptr;
}

Node* Parser::declarationPrime(Node* typeNode, Node* idNode)
{
    try
    {
        if (check({PARENTHESIS_OPEN}))
            return function(typeNode, idNode);
        if (check({SEMICOLON, ASSIGN}))
            return varDecl(typeNode, idNode);
        noMatch({PARENTHESIS_OPEN, SEMICOLON, ASSIGN});
    }
    catch (int err)
    {
        synchronize({INTEGER, BOOLEAN, CHAR, STRING, VOID, END_OF_FILE});
    }
    return nullptr;
}

Node* Parser::function(Node* typeNode, Node* idNode)
{
    try
    {
        if (check({PARENTHESIS_OPEN}))
        {
            terminal(PARENTHESIS_OPEN); auto c1 = params(); terminal(PARENTHESIS_CLOSE);
            terminal(BRACE_OPEN); auto c2 = stmtList(); terminal(BRACE_CLOSE);
            return new Node("FUNCTION", {typeNode, idNode, c1, c2}, (*positionPtr)[curr].first, (*positionPtr)[curr].second);
        }
        noMatch({PARENTHESIS_OPEN});
    }
    catch (int err)
    {
        synchronize({INTEGER, BOOLEAN, CHAR, STRING, VOID, END_OF_FILE});
    }
    return nullptr;
}

Node* Parser::type()
{
    try
    {
        if (check({INTEGER}))
            return terminal(INTEGER), typePrime(new Node("ITYPE", {}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        if (check({BOOLEAN}))
            return terminal(BOOLEAN), typePrime(new Node("BTYPE", {}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        if (check({CHAR}))
            return terminal(CHAR), typePrime(new Node("CTYPE", {}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        if (check({STRING}))
            return terminal(STRING), typePrime(new Node("STYPE", {}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        if (check({VOID}))
            return terminal(VOID), typePrime(new Node("VTYPE", {}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        noMatch({INTEGER, BOOLEAN, CHAR, STRING, VOID});
    }
    catch (int err)
    {
        synchronize({IDENTIFIER});
    }
    return nullptr;
}

Node* Parser::typePrime(Node* typeNode, int cnt)
{
    try
    {
        if (check({BRACKET_OPEN}))
            return terminal(BRACKET_OPEN), terminal(BRACKET_CLOSE), typePrime(typeNode, cnt+1);
        if (check({IDENTIFIER}))
        {
            if (cnt)
                return new Node("ARRAY", {typeNode, new Node("DIMENSIONS", {new Node(std::to_string(cnt), {}, (*positionPtr)[curr].first, (*positionPtr)[curr].second)}, (*positionPtr)[curr].first, (*positionPtr)[curr].second)}, (*positionPtr)[curr].first, (*positionPtr)[curr].second);
            return typeNode;
        }
        noMatch({BRACKET_OPEN, IDENTIFIER});
    }
    catch (int err)
    {
        synchronize({IDENTIFIER});
    }
    return nullptr;
}

Node* Parser::params()
{
    try
    {
        if (check({INTEGER, BOOLEAN, CHAR, STRING, VOID}))
        {
            std::string id;
            auto c = new Node("PARAMETER", {type(), new Node("ID", {new Node((terminal(IDENTIFIER, id), id), {}, (*positionPtr)[curr].first, (*positionPtr)[curr].second)}, (*positionPtr)[curr].first, (*positionPtr)[curr].second)}, (*positionPtr)[curr].first, (*positionPtr)[curr].second);
            return paramsPrime(c);
        }
        if (check({PARENTHESIS_CLOSE}))
            return nullptr;
        noMatch({INTEGER, BOOLEAN, CHAR, STRING, VOID, PARENTHESIS_CLOSE});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE});
    }
    return nullptr;
}

Node* Parser::paramsPrime(Node* firstVal)
{
    try
    {
        if (check({COMMA}))
        {
            terminal(COMMA);
            Node* c = params();
            std::vector<Node*> vec = {firstVal};
            if (c && c->display == "PARAMETERLIST")
                vec.insert(vec.end(), c->ptrs.begin(), c->ptrs.end());
            if (c)
                delete c;
            return new Node("PARAMETERLIST", vec, (*positionPtr)[curr].first, (*positionPtr)[curr].second);
        }
        if (check({PARENTHESIS_CLOSE}))
            return new Node("PARAMETERLIST", {firstVal}, (*positionPtr)[curr].first, (*positionPtr)[curr].second);
        noMatch({COMMA, PARENTHESIS_CLOSE});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE});
    }
    return nullptr;
}

Node* Parser::varDecl(Node* typeNode, Node* idNode)
{
    try
    {
        if (check({SEMICOLON}))
        {
            terminal(SEMICOLON);
            return new Node("VARDECL", {typeNode, idNode, nullptr}, (*positionPtr)[curr].first, (*positionPtr)[curr].second);
        }
        if (check({ASSIGN}))
        {
            terminal(ASSIGN); auto c = expression(); terminal(SEMICOLON);
            return new Node("VARDECL", {typeNode, idNode, c}, (*positionPtr)[curr].first, (*positionPtr)[curr].second);
        }
        noMatch({SEMICOLON, ASSIGN});
    }
    catch (int err)
    {
        synchronize({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, BRACE_CLOSE, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE, END_OF_FILE});
    }
    return nullptr;
}

Node* Parser::stmtList()
{
    try
    {
        if (check({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
        {
            std::vector<Node*> vec = {statement()};
            return stmtListPrime(vec);
        }
        noMatch({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    catch (int err)
    {
        synchronize({BRACE_CLOSE});
    }
    return nullptr;
}

Node* Parser::stmtListPrime(std::vector<Node*>& prevVals)
{
    try
    {
        if (check({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
        {
            prevVals.push_back(statement());
            return stmtListPrime(prevVals);
        }
        if (check({BRACE_CLOSE}))
            return new Node("STMTLIST", prevVals, (*positionPtr)[curr].first, (*positionPtr)[curr].second);
        noMatch({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE, BRACE_CLOSE});
    }
    catch (int err)
    {
        synchronize({BRACE_CLOSE});
    }
    return nullptr;
}

Node* Parser::statement()
{
    try
    {
        if (check({INTEGER, BOOLEAN, CHAR, STRING, VOID}))
        {
            std::string id;
            auto fp = type();
            return varDecl(fp, new Node("ID", {new Node((terminal(IDENTIFIER, id), id), {}, (*positionPtr)[curr].first, (*positionPtr)[curr].second)}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        }
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
        {
            terminal(BRACE_OPEN); auto curr = stmtList(); terminal(BRACE_CLOSE);
            return curr;
        }
        noMatch({INTEGER, BOOLEAN, CHAR, STRING, VOID, IF, FOR, RETURN, IDENTIFIER, PARENTHESIS_OPEN, SEMICOLON, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE, PRINT, BRACE_OPEN});
    }
    catch (int err)
    {
        synchronize({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, BRACE_CLOSE, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    return nullptr;
}

Node* Parser::ifStmt()
{
    try
    {
        if (check({IF}))
        {
            terminal(IF);
            return new Node("IF", {(terminal(PARENTHESIS_OPEN), expression()), (terminal(PARENTHESIS_CLOSE), terminal(BRACE_OPEN), stmtList()), (terminal(BRACE_CLOSE), ifStmtPrime())}, (*positionPtr)[curr].first, (*positionPtr)[curr].second);
        }
        noMatch({IF});
    }
    catch (int err)
    {
        synchronize({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, BRACE_CLOSE, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    return nullptr;
}

Node* Parser::ifStmtPrime()
{
    try
    {
        if (check({ELSE}))
        {
            terminal(ELSE), terminal(BRACE_OPEN); auto c = stmtList(); terminal(BRACE_CLOSE);
            return new Node("ELSE", {c}, (*positionPtr)[curr].first, (*positionPtr)[curr].second);
        }
        if (check({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, BRACE_CLOSE, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
            return nullptr;
        noMatch({ELSE, IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, BRACE_CLOSE, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    catch (int err)
    {
        synchronize({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, BRACE_CLOSE, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    return nullptr;
}

Node* Parser::forStmt()
{
    try
    {
        if (check({FOR}))
        {
            terminal(FOR);
            return new Node("FOR", {(terminal(PARENTHESIS_OPEN), exprStmt()), expression(), (terminal(SEMICOLON), exprStmt()), (terminal(PARENTHESIS_CLOSE), statement())}, (*positionPtr)[curr].first, (*positionPtr)[curr].second);
        }
        noMatch({FOR});
    }
    catch (int err)
    {
        synchronize({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, BRACE_CLOSE, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    return nullptr;
}

Node* Parser::returnStmt()
{
    try
    {
        if (check({RETURN}))
        {
            terminal(RETURN); auto c = expression(); terminal(SEMICOLON);
            return new Node("RETURN", {c}, (*positionPtr)[curr].first, (*positionPtr)[curr].second);
        }
        noMatch({RETURN});
    }
    catch (int err)
    {
        synchronize({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, BRACE_CLOSE, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    return nullptr;
}

Node* Parser::printStmt()
{
    try 
    {
        if (check({PRINT}))
        {
            terminal(PRINT), terminal(PARENTHESIS_OPEN); auto c = exprList(); terminal(PARENTHESIS_CLOSE), terminal(SEMICOLON);
            return new Node("PRINT", {c}, (*positionPtr)[curr].first, (*positionPtr)[curr].second);
        }
        noMatch({PRINT});
    }
    catch (int err)
    {
        synchronize({IDENTIFIER, PARENTHESIS_OPEN, BRACE_OPEN, BRACE_CLOSE, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    return nullptr;
}

Node* Parser::exprStmt()
{
    try
    {
        if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
        {
            auto curr = expression(); terminal(SEMICOLON);
            return curr;
        }
        if (check({SEMICOLON}));
        {
            terminal(SEMICOLON);
            return nullptr;
        }
        noMatch({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE, SEMICOLON});
    }
    catch (int err)
    {
        synchronize({IDENTIFIER, PARENTHESIS_OPEN, PARENTHESIS_CLOSE, BRACE_OPEN, BRACE_CLOSE, INTEGER, BOOLEAN, CHAR, STRING, VOID, SEMICOLON, IF, FOR, RETURN, PRINT, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    return nullptr;
}

Node* Parser::exprList()
{
    try
    {
        if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
            return exprListPrime(expression());
        noMatch({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE});
    }
    return nullptr;
}

Node* Parser::exprListPrime(Node* firstVal)
{
    try
    {
        if (check({COMMA}))
        {
            terminal(COMMA);
            Node* c = exprList();
            std::vector<Node*> vec = {firstVal};
            if (c && c->display == "EXPRLIST")
                vec.insert(vec.end(), c->ptrs.begin(), c->ptrs.end());
            if (c)
                delete c;
            return new Node("EXPRLIST", vec, (*positionPtr)[curr].first, (*positionPtr)[curr].second);
        }
        if (check({PARENTHESIS_CLOSE}))
            return new Node("EXPRLIST", {firstVal}, (*positionPtr)[curr].first, (*positionPtr)[curr].second);
        noMatch({COMMA, PARENTHESIS_CLOSE});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE});        
    }
    return nullptr;
}

Node* Parser::expression()
{
    try
    {
        if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
            return expressionPrime(orExpr());
        noMatch({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON});
    }
    return nullptr;
}

Node* Parser::expressionPrime(Node* firstVal)
{
    try
    {
        if (check({ASSIGN}))
        {
            terminal(ASSIGN);
            return new Node("=", {firstVal, orExpr()}, (*positionPtr)[curr].first, (*positionPtr)[curr].second);
        }
        if (check({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON}))
            return firstVal;
        noMatch({ASSIGN, PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON});
    }
    return nullptr;
}

Node* Parser::orExpr()
{
    try
    {
        if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
            return orExprPrime(andExpr());
        noMatch({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN});
    }
    return nullptr;
}

Node* Parser::orExprPrime(Node* firstVal)
{
    try
    {
        if (check({OR}))
        {
            terminal(OR);
            return orExprPrime(new Node("OR", {firstVal, andExpr()}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        }
        if (check({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN}))
            return firstVal;
        noMatch({OR, PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN});
    }
    return nullptr;
}

Node* Parser::andExpr()
{
    try
    {
        if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
            return andExprPrime(eqExpr());
        noMatch({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR});
    }
    return nullptr;
}

Node* Parser::andExprPrime(Node* firstVal)
{
    try
    {
        if (check({AND}))
        {
            terminal(AND);
            return andExprPrime(new Node("AND", {firstVal, eqExpr()}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        }
        if (check({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR}))
            return firstVal;
        noMatch({AND, PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR});
    }
    return nullptr;
}

Node* Parser::eqExpr()
{
    try
    {
        if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
            return eqExprPrime(relExpr());
        noMatch({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND});
    }
    return nullptr;
}

Node* Parser::eqExprPrime(Node* firstVal)
{
    try
    {
        if (check({EQUAL}))
        {
            terminal(EQUAL);
            return eqExprPrime(new Node("==", {firstVal, relExpr()}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        }
        if (check({NOT_EQUAL}))
        {
            terminal(NOT_EQUAL);
            return eqExprPrime(new Node("!=", {firstVal, relExpr()}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        }
        if (check({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND}))
            return firstVal;
        noMatch({EQUAL, NOT_EQUAL, PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND});
    }
    return nullptr;
}

Node* Parser::relExpr()
{
    try
    {
        if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
            return relExprPrime(expr());
        noMatch({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL});
    }
    return nullptr;
}

Node* Parser::relExprPrime(Node* firstVal)
{
    try
    {
        if (check({LESS_THAN}))
        {
            terminal(LESS_THAN);
            return relExprPrime(new Node("<", {firstVal, expr()}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        }
        if (check({GREATER_THAN}))
        {
            terminal(GREATER_THAN);
            return relExprPrime(new Node(">", {firstVal, expr()}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        }
        if (check({LESS_EQUAL}))
        {
            terminal(LESS_EQUAL);
            return relExprPrime(new Node("<=", {firstVal, expr()}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        }
        if (check({GREATER_EQUAL}))
        {
            terminal(GREATER_EQUAL);
            return relExprPrime(new Node(">=", {firstVal, expr()}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        }
        if (check({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL}))
            return firstVal;
        noMatch({LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL});
    }
    return nullptr;
}

Node* Parser::expr()
{
    try
    {
        if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
            return exprPrime(term());
        noMatch({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL});
    }
    return nullptr;
}

Node* Parser::exprPrime(Node* firstVal)
{
    try
    {
        if (check({PLUS}))
        {
            terminal(PLUS);
            return exprPrime(new Node("+", {firstVal, term()}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        }
        if (check({MINUS}))
        {
            terminal(MINUS);
            return exprPrime(new Node("-", {firstVal, term()}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        }
        if (check({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL}))
            return firstVal;
        noMatch({PLUS, MINUS, PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL});
    }
    return nullptr;
}

Node* Parser::term()
{
    try
    {
        if (check({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
            return termPrime(unary());
        noMatch({IDENTIFIER, PARENTHESIS_OPEN, MINUS, NOT, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS});
    }
    return nullptr;
}

Node* Parser::termPrime(Node* firstVal)
{
    try
    {
        if (check({MULTIPLY}))
        {
            terminal(MULTIPLY);
            return termPrime(new Node("*", {firstVal, unary()}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        }
        if (check({DIVIDE}))
        {
            terminal(DIVIDE);
            return termPrime(new Node("/", {firstVal, unary()}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        }
        if (check({MODULO}))
        {
            terminal(MODULO);
            return termPrime(new Node("%", {firstVal, unary()}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        }
        if (check({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS}))
            return firstVal;
        noMatch({MULTIPLY, DIVIDE, MODULO, PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS});
    }
    return nullptr;
}

Node* Parser::unary()
{
    try
    {
        if (check({NOT}))
        {
            terminal(NOT);
            return new Node("NOT", {unary()}, (*positionPtr)[curr].first, (*positionPtr)[curr].second);
        }
        if (check({MINUS}))
        {
            terminal(MINUS);
            return new Node("NEG", {unary()}, (*positionPtr)[curr].first, (*positionPtr)[curr].second);
        }
        if (check({IDENTIFIER, PARENTHESIS_OPEN, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE}))
            return factor();
        noMatch({NOT, MINUS, IDENTIFIER, PARENTHESIS_OPEN, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS, MULTIPLY, DIVIDE, MODULO});
    }
    return nullptr;
}

Node* Parser::factor()
{
    try
    {
        std::string lit;
        if (check({IDENTIFIER}))
        {
            std::string id;
            auto c = new Node("ID", {new Node((terminal(IDENTIFIER, id), id), {}, (*positionPtr)[curr].first, (*positionPtr)[curr].second)}, (*positionPtr)[curr].first, (*positionPtr)[curr].second);
            return factorPrime2(factorPrime1(c));
        }
        if (check({NUMBER}))
            return factorPrime2(new Node("ILITERAL", {new Node((terminal(NUMBER, lit), lit), {}, (*positionPtr)[curr].first, (*positionPtr)[curr].second)}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        if (check({CHAR_VALUE}))
            return factorPrime2(new Node("CLITERAL", {new Node((terminal(CHAR_VALUE, lit), lit), {}, (*positionPtr)[curr].first, (*positionPtr)[curr].second)}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        if (check({STRING_VALUE}))
            return factorPrime2(new Node("SLITERAL", {new Node((terminal(STRING_VALUE, lit), lit), {}, (*positionPtr)[curr].first, (*positionPtr)[curr].second)}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        if (check({TRUE}))
            return factorPrime2(new Node("BLITERAL", {new Node((terminal(TRUE), "True"), {}, (*positionPtr)[curr].first, (*positionPtr)[curr].second)}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        if (check({FALSE}))
            return factorPrime2(new Node("BLITERAL", {new Node((terminal(FALSE), "False"), {}, (*positionPtr)[curr].first, (*positionPtr)[curr].second)}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        if (check({PARENTHESIS_OPEN}))
        {
            terminal(PARENTHESIS_OPEN); auto curr = expression(); terminal(PARENTHESIS_CLOSE);
            return factorPrime2(curr);
        }
        noMatch({IDENTIFIER, NUMBER, CHAR_VALUE, STRING_VALUE, TRUE, FALSE, PARENTHESIS_OPEN});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS, MULTIPLY, DIVIDE, MODULO});
    }
    return nullptr;
}

Node* Parser::factorPrime1(Node* firstVal)
{
    try
    {
        if (check({PARENTHESIS_OPEN}))
        {
            terminal(PARENTHESIS_OPEN); auto c = exprList(); terminal(PARENTHESIS_CLOSE);
            return new Node("FUNCTIONCALL", {firstVal, c}, (*positionPtr)[curr].first, (*positionPtr)[curr].second);
        }
        if (check({PARENTHESIS_CLOSE, BRACKET_OPEN, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS, MULTIPLY, DIVIDE, MODULO}))
            return firstVal;
        noMatch({PARENTHESIS_OPEN, PARENTHESIS_CLOSE, BRACKET_OPEN, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS, MULTIPLY, DIVIDE, MODULO});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_OPEN, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS, MULTIPLY, DIVIDE, MODULO});
    }
    return nullptr;
}

Node* Parser::factorPrime2(Node* firstVal)
{
    try
    {
        if (check({BRACKET_OPEN}))
        {
            terminal(BRACKET_OPEN); auto c = expression(); terminal(BRACKET_CLOSE);
            return factorPrime2(new Node("ACCESS", {firstVal, c}, (*positionPtr)[curr].first, (*positionPtr)[curr].second));
        }
        if (check({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS, MULTIPLY, DIVIDE, MODULO}))
            return firstVal;
        noMatch({BRACKET_OPEN, PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS, MULTIPLY, DIVIDE, MODULO});
    }
    catch (int err)
    {
        synchronize({PARENTHESIS_CLOSE, BRACKET_CLOSE, COMMA, SEMICOLON, ASSIGN, OR, AND, EQUAL, NOT_EQUAL, LESS_THAN, GREATER_THAN, LESS_EQUAL, GREATER_EQUAL, PLUS, MINUS, MULTIPLY, DIVIDE, MODULO});
    }
    return nullptr;
}

Node* Parser::parse(const std::vector<Token>& tokens,const std::vector<std::pair<int,int>>& position,const std::vector<std::string>& values)
{
    curr = 0;
    tokenPtr = &tokens;
    positionPtr = &position;
    valuesPtr = &values;
    errors = 0;
    Node* returnVal=nullptr;
    
    try
    {
        returnVal = program();
    }
    catch(char c)
    {
        std::cout << "ERROR PARSER: Sudden end of program\n";
    }

    std::cout << "INFO PARSE - Completed with " << errors << " errors.\n";

    if (errors)
        return nullptr;

    return returnVal;  
}
