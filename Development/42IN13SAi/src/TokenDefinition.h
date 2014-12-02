#pragma once

#include <string>

#include "RegexMatcher.h"
#include "MyTokenType.h"

class TokenDefinition
{
public:
    TokenDefinition();
    TokenDefinition(std::string regexString, MyTokenType type);
    TokenDefinition(const TokenDefinition& other);
	~TokenDefinition();
    RegexMatcher matcher;
    MyTokenType myTokenType;
};