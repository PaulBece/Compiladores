#include "Scanner.h"



    std::string Scanner::getTokenName(Token token) {
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

    inline char Scanner::getchar(char* &pos) {
        columnCounter++;
        return *pos++;
    }

    inline char Scanner::peekchar(char* pos) {
        return *pos;
    }

    void Scanner::gettoken(char*& pos, std::vector< Token> &tokens, std::vector<std::string> &values, std::vector<std::pair<int,int>> &position, char* end) {
        end++;
        std::string s;
        char c = getchar(pos);
        s.clear();    while (c == ' ' || c == '\n' || c=='\r' ) {
            if (c == ' ') {
                c = getchar(pos);
            }
            if (c == '\n') {
                c = getchar(pos);
                lineCounter++;
                columnCounter = 1;
            }
            if (c == '\r') {
                c = getchar(pos);
            }
            if (pos>end) return;
        }
        if (c == '[') {
            std::cout << "DEBUG SCAN - " << getTokenName(BRACKET_OPEN) << " found at (" << lineCounter << ":" << columnCounter << ")\n";
            tokens.push_back(BRACKET_OPEN);
            values.push_back(s);
            position.push_back(std::pair<int,int>(lineCounter,columnCounter));
            return;
        }
        if (c == ']') {
            std::cout << "DEBUG SCAN - " << getTokenName(BRACKET_CLOSE) << " found at (" << lineCounter << ":" << columnCounter << ")\n";
            tokens.push_back(BRACKET_CLOSE);
            values.push_back(s);
            position.push_back(std::pair<int,int>(lineCounter,columnCounter));
            return;
        }
        if (c == '(') {
            std::cout << "DEBUG SCAN - " << getTokenName(PARENTHESIS_OPEN) << " found at (" << lineCounter << ":" << columnCounter << ")\n";
            tokens.push_back(PARENTHESIS_OPEN);
            values.push_back(s);
            position.push_back(std::pair<int,int>(lineCounter,columnCounter));
            return;
        }
        if (c == ')') {
            std::cout << "DEBUG SCAN - " << getTokenName(PARENTHESIS_CLOSE) << " found at (" << lineCounter << ":" << columnCounter << ")\n";
            tokens.push_back(PARENTHESIS_CLOSE);
            values.push_back(s);
            position.push_back(std::pair<int,int>(lineCounter,columnCounter));
            return;
        }
        if (c == '+') {
            // if (peekchar(pos) == '+') {
            //     columnCounter++;
            //     pos++;
            //     std::cout << "DEBUG SCAN - " << getTokenName(INC) << " found at (" << lineCounter << ":" << columnCounter-1 << ")\n";
            //     tokens.push_back(INC);
            //     return;
            // }
            std::cout << "DEBUG SCAN - " << getTokenName(PLUS) << " found at (" << lineCounter << ":" << columnCounter << ")\n";
            tokens.push_back(PLUS);
            values.push_back(s);
            position.push_back(std::pair<int,int>(lineCounter,columnCounter));
            return;
        }
        if (c == '-') {
            // if (peekchar(pos) == '-') {
            //     columnCounter++;
            //     pos++;
            //     std::cout << "DEBUG SCAN - " << getTokenName(DEC) << " found at (" << lineCounter << ":" << columnCounter-1 << ")\n";
            //     tokens.push_back(DEC);
            //     return;
            // }
            std::cout << "DEBUG SCAN - " << getTokenName(MINUS) << " found at (" << lineCounter << ":" << columnCounter << ")\n";
            tokens.push_back(MINUS);
            values.push_back(s);
            position.push_back(std::pair<int,int>(lineCounter,columnCounter));
            return;
        }
        if (c == '!') {
            if (peekchar(pos) == '=') {
                columnCounter++;
                pos++;
                std::cout << "DEBUG SCAN - " << getTokenName(NOT_EQUAL) << " found at (" << lineCounter << ":" << columnCounter-1 << ")\n";
                tokens.push_back(NOT_EQUAL);
                values.push_back(s);
                position.push_back(std::pair<int,int>(lineCounter,columnCounter-1));
                return;
            }
            std::cout << "DEBUG SCAN - " << getTokenName(NOT) << " found at (" << lineCounter << ":" << columnCounter << ")\n";
            tokens.push_back(NOT);
            values.push_back(s);
            position.push_back(std::pair<int,int>(lineCounter,columnCounter));
            return;
        }
        if (c == '^') {
            std::cout << "DEBUG SCAN - " << getTokenName(EXPONENT) << " found at (" << lineCounter << ":" << columnCounter << ")\n";
            tokens.push_back(EXPONENT);
            values.push_back(s);
            position.push_back(std::pair<int,int>(lineCounter,columnCounter));
            return;
        }
        if (c == '*') {
            std::cout << "DEBUG SCAN - " << getTokenName(MULTIPLY) << " found at (" << lineCounter << ":" << columnCounter << ")\n";
            tokens.push_back(MULTIPLY);
            values.push_back(s);
            position.push_back(std::pair<int,int>(lineCounter,columnCounter));
            return;
        }
        if (c == '/') {
            if (peekchar(pos) == '/') {
                lineCounter++;
                pos++;
                while (getchar(pos) != '\n' && pos< end);
                columnCounter = 0;
                return ;
            }
            if (peekchar(pos) == '*') {
                pos++;
                while (pos < end) {
                    c = getchar(pos);
                    if (c == '\n') {
                        lineCounter++;
                        columnCounter = 0;
                    }
                    if (c == '*' && peekchar(pos) == '/') {
                        pos++;
                        return;
                    }
                }
                return;
            }
            std::cout << "DEBUG SCAN - " << getTokenName(DIVIDE) << " found at (" << lineCounter << ":" << columnCounter << ")\n";
            tokens.push_back(DIVIDE);
            values.push_back(s);
            position.push_back(std::pair<int,int>(lineCounter,columnCounter));
            return;
        }
        if (c == '%') {
            std::cout << "DEBUG SCAN - " << getTokenName(MODULO) << " found at (" << lineCounter << ":" << columnCounter << ")\n";
            tokens.push_back(MODULO);
            values.push_back(s);
            position.push_back(std::pair<int,int>(lineCounter,columnCounter));
            return;
        }
        if (c == '<') {
            if (peekchar(pos) == '=') {
                columnCounter++;
                pos++;
                std::cout << "DEBUG SCAN - " << getTokenName(LESS_EQUAL) << " found at (" << lineCounter << ":" << columnCounter-1 << ")\n";
                tokens.push_back(LESS_EQUAL);
                values.push_back(s);
                position.push_back(std::pair<int,int>(lineCounter,columnCounter-1));
                return;
            }
            std::cout << "DEBUG SCAN - " << getTokenName(LESS_THAN) << " found at (" << lineCounter << ":" << columnCounter << ")\n";
            tokens.push_back(LESS_THAN);
            values.push_back(s);
            position.push_back(std::pair<int,int>(lineCounter,columnCounter));
            return;
        }
        if (c == '>') {
            if (peekchar(pos) == '=') {
                columnCounter++;
                pos++;
                std::cout << "DEBUG SCAN - " << getTokenName(GREATER_EQUAL) << " found at (" << lineCounter << ":" << columnCounter-1 << ")\n";
                tokens.push_back(GREATER_EQUAL);
                values.push_back(s);
                position.push_back(std::pair<int,int>(lineCounter,columnCounter-1));
                return;
            }
            std::cout << "DEBUG SCAN - " << getTokenName(GREATER_THAN) << " found at (" << lineCounter << ":" << columnCounter << ")\n";
            tokens.push_back(GREATER_THAN);
            values.push_back(s);
            position.push_back(std::pair<int,int>(lineCounter,columnCounter));
            return;
        }
        if (c == '=') {
            if (peekchar(pos) == '=') {
                columnCounter++;
                pos++;
                std::cout << "DEBUG SCAN - " << getTokenName(EQUAL) << " found at (" << lineCounter << ":" << columnCounter-1 << ")\n";
                tokens.push_back(EQUAL);
                values.push_back(s);
                position.push_back(std::pair<int,int>(lineCounter,columnCounter-1));
                return;
            }
            std::cout << "DEBUG SCAN - " << getTokenName(ASSIGN) << " found at (" << lineCounter << ":" << columnCounter << ")\n";
            tokens.push_back(ASSIGN);
            values.push_back(s);
            position.push_back(std::pair<int,int>(lineCounter,columnCounter));
            return;
        }
        if (c == '{') {
            std::cout << "DEBUG SCAN - " << getTokenName(BRACE_OPEN) << " found at (" << lineCounter << ":" << columnCounter << ")\n";
            tokens.push_back(BRACE_OPEN);
            values.push_back(s);
            position.push_back(std::pair<int,int>(lineCounter,columnCounter));
            return;
        }
        if (c == '}') {
            std::cout << "DEBUG SCAN - " << getTokenName(BRACE_CLOSE) << " found at (" << lineCounter << ":" << columnCounter << ")\n";
            tokens.push_back(BRACE_CLOSE);
            values.push_back(s);
            position.push_back(std::pair<int,int>(lineCounter,columnCounter));
            return;
        }
        if (c == '&') {
            if (peekchar(pos) == '&') {
                columnCounter++;
                pos++;
                std::cout << "DEBUG SCAN - " << getTokenName(AND) << " found at (" << lineCounter << ":" << columnCounter-1 << ")\n";
                tokens.push_back(AND);
                values.push_back(s);
                position.push_back(std::pair<int,int>(lineCounter,columnCounter-1));
                return;
            }
            else {
                errorCounter++;            
                s += c;
                std::cout << "DEBUG SCAN - " << "Invalid token (" << s << ") at (" << lineCounter << ":" << columnCounter << ")\n";
                return;
            }
        }
        if (c == '|') {
            if (peekchar(pos) == '|') {
                columnCounter++;
                pos++;
                std::cout << "DEBUG SCAN - " << getTokenName(OR) << " found at (" << lineCounter << ":" << columnCounter-1 << ")\n";
                tokens.push_back(OR);
                values.push_back(s);
                position.push_back(std::pair<int,int>(lineCounter,columnCounter-1));
                return;
            }
            else {
                errorCounter++;            
                s += c;
                std::cout << "DEBUG SCAN - " << "Invalid token (" << s << ") at (" << lineCounter << ":" << columnCounter << ")\n";
                return;
            }
        }
        if (c == ':') {
            std::cout << "DEBUG SCAN - " << getTokenName(COLON) << " found at (" << lineCounter << ":" << columnCounter << ")\n";
            tokens.push_back(COLON);
            values.push_back(s);
            position.push_back(std::pair<int,int>(lineCounter,columnCounter));
            return;
        }
        if (c == ',') {
            std::cout << "DEBUG SCAN - " << getTokenName(COMMA) << " found at (" << lineCounter << ":" << columnCounter << ")\n";
            tokens.push_back(COMMA);
            values.push_back(s);
            position.push_back(std::pair<int,int>(lineCounter,columnCounter));
            return;
        }
        if (c == ';') {
            std::cout << "DEBUG SCAN - " << getTokenName(SEMICOLON) << " found at (" << lineCounter << ":" << columnCounter << ")\n";
            tokens.push_back(SEMICOLON);
            values.push_back(s);
            position.push_back(std::pair<int,int>(lineCounter,columnCounter));
            return;
        }
        if (c == 39) {
            s+=c;
            c=getchar(pos);
            if (c==39) {
                errorCounter++;
                pos--;
                columnCounter--;
                std::cout << "DEBUG SCAN - " << "Invalid token (" << s << ") at (" << lineCounter << ":" << columnCounter - s.size()+1 << ")\n";
                return;
            }
            if (c=='\r' || c=='\n'){
                errorCounter++;
                std::cout << "DEBUG SCAN - " << "Invalid token (" << s << ") at (" << lineCounter << ":" << columnCounter - s.size() << ")\n";
                pos--;
                return;
            }
            if (peekchar(pos)==39) {
                s+=c;
                c=getchar(pos);
                s+=c;
                std::cout << "DEBUG SCAN - " << getTokenName(CHAR_VALUE) << " [" << s << "] found at (" << lineCounter << ":" << columnCounter-s.size()+1 << ")\n";
                tokens.push_back(CHAR_VALUE);
                values.push_back(s);
                position.push_back(std::pair<int,int>(lineCounter,columnCounter-s.size()+1));
                return;
            }
            errorCounter++;
            pos--;
            columnCounter--;
            std::cout << "DEBUG SCAN - " << "Invalid token (" << s << ") at (" << lineCounter << ":" << columnCounter - s.size()+1 << ")\n";
            return;
        }
        if (c == 34) {
            s+=c;
            c = getchar(pos);
            while (c != '"' && pos <end) {
                if (c=='\r' || c=='\n'){
                    errorCounter++;
                    std::cout << "DEBUG SCAN - " << "Invalid token (" << s << ") at (" << lineCounter << ":" << columnCounter - s.size() << ")\n";
                    pos--;
                    return;
                }
                s += c;
                c = getchar(pos);
            }
            s+=c;
            std::cout << "DEBUG SCAN - " << getTokenName(STRING_VALUE) << " [" << s << "] found at (" << lineCounter << ":" << columnCounter-s.size()+1 << ")\n";
            tokens.push_back(STRING_VALUE);
            values.push_back(s);
            position.push_back(std::pair<int,int>(lineCounter,columnCounter-s.size()+1));
            return;
        }
        if (c >= '0' && c <= '9') {
            s += c;
            c = getchar(pos);
            while (c >= '0' && c <= '9' && pos< end) {
                s += c;
                c = getchar(pos);
            }
            pos--;
            columnCounter--;
            if (s.size()==20){
                if (s<="18446744073709551615") {
                    std::cout << "DEBUG SCAN - " << getTokenName(NUMBER) << " [" << s << "] found at (" << lineCounter << ":" << columnCounter-s.size()+1 << ")\n";
                    tokens.push_back(NUMBER);
                    values.push_back(s);
                    position.push_back(std::pair<int,int>(lineCounter,columnCounter-s.size()+1));
                }
                else{
                    errorCounter++;
                    std::cout << "DEBUG SCAN - " << "Max value of integer exceeded (" << s << ") at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
                }
                return;
            }
            else if (s.size()<20){
                std::cout << "DEBUG SCAN - " << getTokenName(NUMBER) << " [" << s << "] found at (" << lineCounter << ":" << columnCounter-s.size()+1 << ")\n";
                tokens.push_back(NUMBER);
                values.push_back(s);
                position.push_back(std::pair<int,int>(lineCounter,columnCounter-s.size()+1));
                return;
            }
            else{
                errorCounter++;
                std::cout << "DEBUG SCAN - " << "Max value of integer exceeded (" << s << ") at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
                return;
            }
            
        }
        if (c >= 65 && c <= 90 || c >= 97 && c <= 122 || c == '_') {
            s += c;
            c = getchar(pos);
            while (c >= 65 && c <= 90 || c >= 97 && c <= 122 || c == '_' || c >= '0' && c <= '9' && pos < end) {
                s += c;
                c = getchar(pos);
            }
            pos--;
            columnCounter--;
            if (s == "integer") {
                std::cout << "DEBUG SCAN - " << getTokenName(INTEGER) << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
                tokens.push_back(INTEGER);
                values.push_back(s);
                position.push_back(std::pair<int,int>(lineCounter,columnCounter-s.size()+1));
                return;
            }
            if (s == "boolean") {
                std::cout << "DEBUG SCAN - " << getTokenName(BOOLEAN) << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
                tokens.push_back(BOOLEAN);
                values.push_back(s);
                position.push_back(std::pair<int,int>(lineCounter,columnCounter-s.size()+1));
                return;
            }
            if (s == "char") {
                std::cout << "DEBUG SCAN - " << getTokenName(CHAR) << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
                tokens.push_back(CHAR);
                values.push_back(s);
                position.push_back(std::pair<int,int>(lineCounter,columnCounter-s.size()+1));
                return;
            }
            if (s == "string") {
                std::cout << "DEBUG SCAN - " << getTokenName(STRING) << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
                tokens.push_back(STRING);
                values.push_back(s);
                position.push_back(std::pair<int,int>(lineCounter,columnCounter-s.size()+1));
                return;
            }
            if (s == "false") {
                std::cout << "DEBUG SCAN - " << getTokenName(FALSE) << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
                tokens.push_back(FALSE);
                values.push_back(s);
                position.push_back(std::pair<int,int>(lineCounter,columnCounter-s.size()+1));
                return;
            }
            if (s == "true") {
                std::cout << "DEBUG SCAN - " << getTokenName(TRUE) << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
                tokens.push_back(TRUE);
                values.push_back(s);
                position.push_back(std::pair<int,int>(lineCounter,columnCounter-s.size()+1));
                return;
            }
            if (s == "array") {
                std::cout << "DEBUG SCAN - " << getTokenName(ARRAY) << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
                tokens.push_back(ARRAY);
                values.push_back(s);
                position.push_back(std::pair<int,int>(lineCounter,columnCounter-s.size()+1));
                return;
            }
            if (s == "else") {
                std::cout << "DEBUG SCAN - " << getTokenName(ELSE) << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
                tokens.push_back(ELSE);
                values.push_back(s);
                position.push_back(std::pair<int,int>(lineCounter,columnCounter-s.size()+1));
                return;
            }
            if (s == "for") {
                std::cout << "DEBUG SCAN - " << getTokenName(FOR) << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
                tokens.push_back(FOR);
                values.push_back(s);
                position.push_back(std::pair<int,int>(lineCounter,columnCounter-s.size()+1));
                return;
            }
            if (s == "function") {
                std::cout << "DEBUG SCAN - " << getTokenName(FUNCTION) << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
                tokens.push_back(FUNCTION);
                values.push_back(s);
                position.push_back(std::pair<int,int>(lineCounter,columnCounter-s.size()+1));
                return;
            }
            if (s == "if") {
                std::cout << "DEBUG SCAN - " << getTokenName(IF) << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
                tokens.push_back(IF);
                values.push_back(s);
                position.push_back(std::pair<int,int>(lineCounter,columnCounter-s.size()+1));
                return;
            }
            if (s == "print") {
                std::cout << "DEBUG SCAN - " << getTokenName(PRINT) << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
                tokens.push_back(PRINT);
                values.push_back(s);
                position.push_back(std::pair<int,int>(lineCounter,columnCounter-s.size()+1));
                return;
            }
            if (s == "return") {
                std::cout << "DEBUG SCAN - " << getTokenName(RETURN) << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
                tokens.push_back(RETURN);
                values.push_back(s);
                position.push_back(std::pair<int,int>(lineCounter,columnCounter-s.size()+1));
                return;
            }
            if (s == "void") {
                std::cout << "DEBUG SCAN - " << getTokenName(VOID) << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
                tokens.push_back(VOID);
                values.push_back(s);
                position.push_back(std::pair<int,int>(lineCounter,columnCounter-s.size()+1));
                return;
            }
            if (s == "while") {
                std::cout << "DEBUG SCAN - " << getTokenName(WHILE) << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
                tokens.push_back(WHILE);
                values.push_back(s);
                position.push_back(std::pair<int,int>(lineCounter,columnCounter-s.size()+1));
                return;
            }
            std::cout << "DEBUG SCAN - " << getTokenName(IDENTIFIER) << " ["<<s<<"] found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
            tokens.push_back(IDENTIFIER);
            values.push_back(s);
            position.push_back(std::pair<int,int>(lineCounter,columnCounter-s.size()+1));
            return;
        }
        errorCounter++;
        s += c;
        std::cout << "DEBUG SCAN - " << "Invalid token (" << s << ") at (" << lineCounter << ":" << columnCounter << ")\n";
    }

