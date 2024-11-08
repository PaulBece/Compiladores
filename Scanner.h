#include "common.h"

class Scanner{
    public:
    int lineCounter = 1;
    int columnCounter = 0;
    int errorCounter = 0;

    //std::string getTokenName(Token token) ;
    inline char getchar(char* &pos) ;

    inline char peekchar(char* pos) ;

    void gettoken(char*& pos, std::vector< Token> &tokens, std::vector<std::string> &values,std::vector<std::pair<int,int>> &position, char* end) ;

};