//
//  TokenPartner.h
//  tokenizer
//
//  Created by Alex Nagelkerke on 19-09-14.
//  Copyright (c) 2014 Alex Nagelkerke. All rights reserved.
//

#ifndef TOKENPARTNER_H_INCLUDED
#define TOKENPARTNER_H_INCLUDED

#include <string>
#include "TokenType.h"

class TokenPartner
{
public:
    TokenType token;
    TokenType partner;
    TokenPartner(TokenType tokenStr, TokenType partnerStr);
};

#endif