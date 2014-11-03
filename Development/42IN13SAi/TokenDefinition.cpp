#include <stdio.h>

#include "TokenDefinition.h"

TokenDefinition::TokenDefinition(std::string regexString, TokenType type) : matcher(RegexMatcher(regexString)), tokenType(type)
{
}

TokenDefinition::~TokenDefinition()
{
}