#pragma once

#include <string>
#include "MyTokenType.h"


class Token
{
public:
    int LineNumber;
    int LinePosition;
    int Level;
    std::string Value;
    MyTokenType Type;
	Token* Partner;
    
    Token(int lineNumber, int linePosition, int level, std::string value, MyTokenType myTokenType, Token* partner);
    Token();
	~Token();
};