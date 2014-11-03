#pragma once

#include <string>

#include "RegexMatcher.h"
#include "TokenType.h"

class TokenDefinition
{
public:
    TokenDefinition();
    TokenDefinition(std::string regexString, TokenType type);
    TokenDefinition(const TokenDefinition& other);
	~TokenDefinition();
    RegexMatcher matcher;
    TokenType tokenType;
};