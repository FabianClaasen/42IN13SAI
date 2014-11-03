#include <stdio.h>

#include "TokenDefinition.h"

TokenDefinition::TokenDefinition()
{};

TokenDefinition::TokenDefinition(std::string regexString, TokenType type) : matcher(RegexMatcher(regexString)), tokenType(type)
{
}

TokenDefinition::TokenDefinition(const TokenDefinition& other) :matcher(other.matcher), tokenType(other.tokenType)
{
}

TokenDefinition::~TokenDefinition()
{
}