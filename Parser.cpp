#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "common.h"

std::vector<Token> tokens;

int currSymbol = 0;

void error(std::string msg)
{
    std::cout << "Error: " << msg << '\n';
}

bool accept