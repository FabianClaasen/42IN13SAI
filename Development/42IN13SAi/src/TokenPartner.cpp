#include <stdio.h>

#include "TokenPartner.h"

TokenPartner::TokenPartner(MyTokenType tokenStr, MyTokenType partnerStr)
{
    token   = tokenStr;
    partner = partnerStr;
}