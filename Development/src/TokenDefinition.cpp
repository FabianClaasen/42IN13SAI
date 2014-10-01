//
//  TokenDefinition.cpp
//  tokenizer
//
//  Created by Alex Nagelkerke on 19-09-14.
//  Copyright (c) 2014 Alex Nagelkerke. All rights reserved.
//

#include <stdio.h>
#include "TokenDefinition.h"

TokenDefinition::TokenDefinition(std::string regexString, TokenType type)
{
    matcher = new RegexMatcher(regexString);
    tokenType = type;
}
