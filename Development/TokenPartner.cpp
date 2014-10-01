//
//  TokenPartner.cpp
//  tokenizer
//
//  Created by Alex Nagelkerke on 19-09-14.
//  Copyright (c) 2014 Alex Nagelkerke. All rights reserved.
//

#include <stdio.h>
#include "TokenPartner.h"

TokenPartner::TokenPartner(TokenType tokenStr, TokenType partnerStr)
{
    token   = tokenStr;
    partner = partnerStr;
}