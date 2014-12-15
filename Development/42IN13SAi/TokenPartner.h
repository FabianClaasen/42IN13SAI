///
/// @file MyTokenType.h
/// @brief Contains the data for a token partner
/// @ingroup Tokenizer
///
#pragma once

#include <string>

#include "MyTokenType.h"

class TokenPartner
{
public:
    /// The MyTokenType of the main token
    ///
    MyTokenType token;
    
    /// The MyTokenType of the token that is a partner to the main token
    ///
    MyTokenType partner;
    
    /// The constructor with the main and partner type
    ///
    /// @param tokenStr The main MyTokenType
    /// @param partnerStr The partner MyTokenType
    ///
    TokenPartner(MyTokenType tokenStr, MyTokenType partnerStr);
};