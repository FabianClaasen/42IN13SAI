#include <stdio.h>

#include "TokenDefinition.h"

TokenDefinition::TokenDefinition(std::string regexString, TokenType type)
{
    matcher = new RegexMatcher(regexString);
    tokenType = type;
}
