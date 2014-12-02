#include <stdio.h>

#include "TokenDefinition.h"

TokenDefinition::TokenDefinition()
{};

TokenDefinition::TokenDefinition(std::string regexString, MyTokenType type) : matcher(RegexMatcher(regexString)), myTokenType(type)
{
}

TokenDefinition::TokenDefinition(const TokenDefinition& other) :matcher(other.matcher), myTokenType(other.myTokenType)
{
}

TokenDefinition::~TokenDefinition()
{
}