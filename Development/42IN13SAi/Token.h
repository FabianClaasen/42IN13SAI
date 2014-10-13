#pragma once

#include <string>
#include "TokenType.h"

class Token
{
public:
    int LineNumber;
    int LinePosition;
    int Level;
    std::string Value;
    TokenType Type;
    Token *Partner;
    
    Token(int lineNumber, int linePosition, int level, std::string value, TokenType tokenType, Token *partner);
    Token();
};