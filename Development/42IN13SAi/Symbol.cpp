#include "Symbol.h"

Symbol::Symbol() :name(""), type(TokenType::None), kind(SymbolKind::None)
{
    
}

Symbol::Symbol(std::string p_name, TokenType p_type, SymbolKind p_kind) :name(p_name), type(p_type), kind(p_kind)
{
}

Symbol::~Symbol()
{
    
}

Symbol& Symbol::operator=(const Symbol& other)
{
    if (this != &other)
    {
        Symbol* cSymbol = new Symbol(other);
        return *cSymbol;
        
    }
    return *this;
}

void Symbol::SetValue(float p_value)
{
	value = p_value;
}

float Symbol::GetValue()
{
    return value;
}