#pragma once

#include <string>

#include "TokenType.h"

class TokenPartner
{
public:
    TokenType token; 
    TokenType partner;
    TokenPartner(TokenType tokenStr, TokenType partnerStr);
};