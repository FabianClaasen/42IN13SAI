//
//  Symbol.cpp
//  mac_42IN13SAI
//
//  Copyright (c) 2014 42IN13SAI. All rights reserved.
//

#include "Symbol.h"

Symbol::Symbol() :name(""), type(TokenType::EOL), kind(SymbolKind::None)
{
    
}

Symbol::Symbol(std::string p_name, TokenType p_type, SymbolKind p_kind) :name(p_name), type(p_type), kind(p_kind)
{
}

Symbol::~Symbol()
{
    
}