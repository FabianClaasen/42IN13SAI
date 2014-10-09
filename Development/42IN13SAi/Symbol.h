//
//  Symbol.h
//  mac_42IN13SAI
//

#pragma once

#include <string>
#include "TokenType.h"
#include "SymbolKind.h"

class Symbol
{
private:
public:
    const std::string name;
    const TokenType type;
    const SymbolKind kind;
    
    Symbol();
    Symbol(std::string p_name, TokenType p_type, SymbolKind p_kind);
    virtual ~Symbol();
    
    float GetValue();
    void SetValue(float p_value);
};