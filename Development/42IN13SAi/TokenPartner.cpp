//
//  TokenPartner.cpp
//  tokenizer
//
#include <stdio.h>
#include "TokenPartner.h"

TokenPartner::TokenPartner(TokenType tokenStr, TokenType partnerStr)
{
    token   = tokenStr;
    partner = partnerStr;
}