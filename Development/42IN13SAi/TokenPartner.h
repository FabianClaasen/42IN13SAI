#pragma once

#include <string>

#include "MyTokenType.h"

class TokenPartner
{
public:
    MyTokenType token; 
    MyTokenType partner;
    TokenPartner(MyTokenType tokenStr, MyTokenType partnerStr);
};