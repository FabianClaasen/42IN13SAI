#pragma once

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