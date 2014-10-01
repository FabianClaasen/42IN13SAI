//
//  Token.h
//  tokenizer
//
//  Created by Alex Nagelkerke on 19-09-14.
//  Copyright (c) 2014 Alex Nagelkerke. All rights reserved.
//
#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED

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

#endif