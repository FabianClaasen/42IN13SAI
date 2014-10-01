//
//  TokenDefinition.h
//  tokenizer
//
//  Created by Alex Nagelkerke on 19-09-14.
//  Copyright (c) 2014 Alex Nagelkerke. All rights reserved.
//

#ifndef TOKENDEFINITION_H_INCLUDED
#define TOKENDEFINITION_H_INCLUDED

#include <string>
#include "RegexMatcher.h"
#include "TokenType.h"

class TokenDefinition
{
public:
    TokenDefinition(std::string regexString, TokenType type);
    RegexMatcher *matcher;
    TokenType tokenType;
};

#endif