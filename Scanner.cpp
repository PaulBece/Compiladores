#include <iostream>
#include <vector>
#include <string>
#include <fstream>

enum Token {
    BRACKET_OPEN,       // [
    BRACKET_CLOSE,      // ]
    INC,                // ++ (postfix increment)
    PARENTHESIS_OPEN,   // (
    PARENTHESIS_CLOSE,  // )
    DEC,                // -- (postfix decrement)
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
    SINGLE_QUOTE,       // '
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
    WHILE               //while
};

int lineCounter = 1;
int columnCounter = 0;
int errorCounter = 0;

inline char getchar(char* &pos) {
    columnCounter++;
    return *pos++;
}

inline char peekchar(char* pos) {
    return *pos;
}
void gettoken(char*& pos, std::vector< Token> &tokens, std::vector<std::string> &values, char* end) {
    end++;
    std::string s;
    char c = getchar(pos);
    s.clear();    while (c == ' ' || c == '\n' || c=='\r') {
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
    }
    if (c == '[') {
        std::cout << "DEBUG SCAN - " << BRACKET_OPEN << " found at (" << lineCounter << ":" << columnCounter << ")\n";
        tokens.push_back(BRACKET_OPEN);
        return;
    }
    if (c == ']') {
        std::cout << "DEBUG SCAN - " << BRACKET_CLOSE << " found at (" << lineCounter << ":" << columnCounter << ")\n";
        tokens.push_back(BRACKET_CLOSE);
        return;
    }
    if (c == '(') {
        std::cout << "DEBUG SCAN - " << PARENTHESIS_OPEN << " found at (" << lineCounter << ":" << columnCounter << ")\n";
        tokens.push_back(PARENTHESIS_OPEN);
        return;
    }
    if (c == ')') {
        std::cout << "DEBUG SCAN - " << PARENTHESIS_CLOSE << " found at (" << lineCounter << ":" << columnCounter << ")\n";
        tokens.push_back(PARENTHESIS_CLOSE);
        return;
    }
    if (c == '+') {
        if (peekchar(pos) == '+') {
            columnCounter++;
            pos++;
            std::cout << "DEBUG SCAN - " << INC << " found at (" << lineCounter << ":" << columnCounter-1 << ")\n";
            tokens.push_back(INC);
            return;
        }
        std::cout << "DEBUG SCAN - " << PLUS << " found at (" << lineCounter << ":" << columnCounter << ")\n";
        tokens.push_back(PLUS);
        return;
    }
    if (c == '-') {
        if (peekchar(pos) == '-') {
            columnCounter++;
            pos++;
            std::cout << "DEBUG SCAN - " << DEC << " found at (" << lineCounter << ":" << columnCounter-1 << ")\n";
            tokens.push_back(DEC);
            return;
        }
        std::cout << "DEBUG SCAN - " << MINUS << " found at (" << lineCounter << ":" << columnCounter << ")\n";
        tokens.push_back(MINUS);
        return;
    }
    if (c == '!') {
        if (peekchar(pos) == '=') {
            columnCounter++;
            pos++;
            std::cout << "DEBUG SCAN - " << NOT_EQUAL << " found at (" << lineCounter << ":" << columnCounter-1 << ")\n";
            tokens.push_back(NOT_EQUAL);
            return;
        }
        std::cout << "DEBUG SCAN - " << NOT << " found at (" << lineCounter << ":" << columnCounter << ")\n";
        tokens.push_back(NOT);
        return;
    }
    if (c == '^') {
        std::cout << "DEBUG SCAN - " << EXPONENT << " found at (" << lineCounter << ":" << columnCounter << ")\n";
        tokens.push_back(EXPONENT);
        return;
    }
    if (c == '*') {
        std::cout << "DEBUG SCAN - " << MULTIPLY << " found at (" << lineCounter << ":" << columnCounter << ")\n";
        tokens.push_back(MULTIPLY);
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
        std::cout << "DEBUG SCAN - " << DIVIDE << " found at (" << lineCounter << ":" << columnCounter << ")\n";
        tokens.push_back(DIVIDE);
        return;
    }
    if (c == '%') {
        std::cout << "DEBUG SCAN - " << MODULO << " found at (" << lineCounter << ":" << columnCounter << ")\n";
        tokens.push_back(MODULO);
        return;
    }
    if (c == '<') {
        if (peekchar(pos) == '=') {
            columnCounter++;
            pos++;
            std::cout << "DEBUG SCAN - " << LESS_EQUAL << " found at (" << lineCounter << ":" << columnCounter-1 << ")\n";
            tokens.push_back(LESS_EQUAL);
            return;
        }
        std::cout << "DEBUG SCAN - " << LESS_THAN << " found at (" << lineCounter << ":" << columnCounter << ")\n";
        tokens.push_back(LESS_THAN);
        return;
    }
    if (c == '>') {
        if (peekchar(pos) == '=') {
            columnCounter++;
            pos++;
            std::cout << "DEBUG SCAN - " << GREATER_EQUAL << " found at (" << lineCounter << ":" << columnCounter-1 << ")\n";
            tokens.push_back(GREATER_EQUAL);
            return;
        }
        std::cout << "DEBUG SCAN - " << GREATER_THAN << " found at (" << lineCounter << ":" << columnCounter << ")\n";
        tokens.push_back(GREATER_THAN);
        return;
    }
    if (c == '=') {
        if (peekchar(pos) == '=') {
            columnCounter++;
            pos++;
            std::cout << "DEBUG SCAN - " << EQUAL << " found at (" << lineCounter << ":" << columnCounter-1 << ")\n";
            tokens.push_back(EQUAL);
            return;
        }
        std::cout << "DEBUG SCAN - " << ASSIGN << " found at (" << lineCounter << ":" << columnCounter << ")\n";
        tokens.push_back(ASSIGN);
        return;
    }
    if (c == '{') {
        std::cout << "DEBUG SCAN - " << BRACE_OPEN << " found at (" << lineCounter << ":" << columnCounter << ")\n";
        tokens.push_back(BRACE_OPEN);
        return;
    }
    if (c == '}') {
        std::cout << "DEBUG SCAN - " << BRACE_CLOSE << " found at (" << lineCounter << ":" << columnCounter << ")\n";
        tokens.push_back(BRACE_CLOSE);
        return;
    }
    if (c == '&') {
        if (peekchar(pos) == '&') {
            columnCounter++;
            pos++;
            std::cout << "DEBUG SCAN - " << AND << " found at (" << lineCounter << ":" << columnCounter-1 << ")\n";
            tokens.push_back(AND);
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
            std::cout << "DEBUG SCAN - " << OR << " found at (" << lineCounter << ":" << columnCounter-1 << ")\n";
            tokens.push_back(OR);
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
        std::cout << "DEBUG SCAN - " << COLON << " found at (" << lineCounter << ":" << columnCounter << ")\n";
        tokens.push_back(COLON);
        return;
    }
    if (c == ',') {
        std::cout << "DEBUG SCAN - " << COMMA << " found at (" << lineCounter << ":" << columnCounter << ")\n";
        tokens.push_back(COMMA);
        return;
    }
    if (c == ';') {
        std::cout << "DEBUG SCAN - " << SEMICOLON << " found at (" << lineCounter << ":" << columnCounter << ")\n";
        tokens.push_back(SEMICOLON);
        return;
    }
    if (c == 39) {
        std::cout << "DEBUG SCAN - " << SINGLE_QUOTE << " found at (" << lineCounter << ":" << columnCounter << ")\n";
        tokens.push_back(SINGLE_QUOTE);
        return;
    }
    if (c == 34) {
        s+=c;
        c = getchar(pos);
        while (c != '"') {
            if (c=='\r'){
                errorCounter++;
                std::cout << "DEBUG SCAN - " << "Invalid token (" << s << ") at (" << lineCounter << ":" << columnCounter - s.size() << ")\n";
                pos--;
                return;
            }
            s += c;
            c = getchar(pos);
        }
        s+=c;
        std::cout << "DEBUG SCAN - " << STRING_VALUE << " [" << s << "] found at (" << lineCounter << ":" << columnCounter-s.size()+1 << ")\n";
        tokens.push_back(STRING_VALUE);
        return;
    }
    if (c >= '0' && c <= '9') {
        s += c;
        c = getchar(pos);
        while (c >= '0' && c <= '9') {
            s += c;
            c = getchar(pos);
        }
        pos--;
        columnCounter--;
        if (s.size()==20){
            if (s<="18446744073709551615") {
                std::cout << "DEBUG SCAN - " << NUMBER << " [" << s << "] found at (" << lineCounter << ":" << columnCounter-s.size()+1 << ")\n";
                tokens.push_back(NUMBER);
                values.push_back(s);
            }
            else{
                errorCounter++;
                std::cout << "DEBUG SCAN - " << "Max value of integer exceeded (" << s << ") at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
            }
            return;
        }
        else if (s.size()<20){
            std::cout << "DEBUG SCAN - " << NUMBER << " [" << s << "] found at (" << lineCounter << ":" << columnCounter-s.size()+1 << ")\n";
            tokens.push_back(NUMBER);
            values.push_back(s);
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
        while (c >= 65 && c <= 90 || c >= 97 && c <= 122 || c == '_' || c >= '0' && c <= '9') {
            s += c;
            c = getchar(pos);
        }
        pos--;
        columnCounter--;
        if (s == "integer") {
            std::cout << "DEBUG SCAN - " << INTEGER << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
            tokens.push_back(INTEGER);
            return;
        }
        if (s == "boolean") {
            std::cout << "DEBUG SCAN - " << BOOLEAN << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
            tokens.push_back(BOOLEAN);
            return;
        }
        if (s == "char") {
            std::cout << "DEBUG SCAN - " << CHAR << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
            tokens.push_back(CHAR);
            return;
        }
        if (s == "string") {
            std::cout << "DEBUG SCAN - " << STRING << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
            tokens.push_back(STRING);
            return;
        }
        if (s == "false") {
            std::cout << "DEBUG SCAN - " << FALSE << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
            tokens.push_back(FALSE);
            return;
        }
        if (s == "true") {
            std::cout << "DEBUG SCAN - " << TRUE << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
            tokens.push_back(TRUE);
            return;
        }
        if (s == "array") {
            std::cout << "DEBUG SCAN - " << TRUE << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
            tokens.push_back(ARRAY);
            return;
        }
        if (s == "else") {
            std::cout << "DEBUG SCAN - " << TRUE << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
            tokens.push_back(ELSE);
            return;
        }
        if (s == "for") {
            std::cout << "DEBUG SCAN - " << TRUE << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
            tokens.push_back(FOR);
            return;
        }
        if (s == "function") {
            std::cout << "DEBUG SCAN - " << TRUE << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
            tokens.push_back(FUNCTION);
            return;
        }
        if (s == "if") {
            std::cout << "DEBUG SCAN - " << TRUE << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
            tokens.push_back(IF);
            return;
        }
        if (s == "print") {
            std::cout << "DEBUG SCAN - " << TRUE << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
            tokens.push_back(PRINT);
            return;
        }
        if (s == "return") {
            std::cout << "DEBUG SCAN - " << TRUE << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
            tokens.push_back(RETURN);
            return;
        }
        if (s == "void") {
            std::cout << "DEBUG SCAN - " << TRUE << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
            tokens.push_back(VOID);
            return;
        }
        if (s == "while") {
            std::cout << "DEBUG SCAN - " << TRUE << " found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
            tokens.push_back(WHILE);
            return;
        }
        std::cout << "DEBUG SCAN - " << IDENTIFIER << " ["<<s<<"] found at (" << lineCounter << ":" << columnCounter - s.size() + 1 << ")\n";
        tokens.push_back(IDENTIFIER);
        values.push_back(s);
        return;
    }
    errorCounter++;
    s += c;
    std::cout << "DEBUG SCAN - " << "Invalid token (" << s << ") at (" << lineCounter << ":" << columnCounter << ")\n";
}

int main()
{
    const std::string filePath = "test.txt";
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);

    if (!file) {
        std::cerr << "Unable to open file: " << filePath << std::endl;
        return 1;
    }

    std::streamsize fileSize = file.tellg();

    std::vector<char> buffer(fileSize);

    file.seekg(0, std::ios::beg);

    if (file.read(buffer.data(), fileSize)) {
        std::cout << "File loaded successfully." << std::endl;
    }
    else {
        std::cerr << "Error reading the file." << std::endl;
    }

    file.close();

    

    char* pos = &buffer[0];
    std::vector<Token> tokens;
    std::vector<std::string> values;

    std::cout << "INFO SCAN - Start scanning...\n";

    while (pos <= &buffer[buffer.size()-1]) {
        gettoken(pos,tokens,values, &buffer[buffer.size()-1]);
    }
    
    std::cout << "INFO SCAN - Completed with "<<errorCounter<<" errors\n";

    return 0;
}