#include "Symbol.h"

Symbol::Symbol() :name(""), type(MyTokenType::None), kind(SymbolKind::None), is_set(false)
{
    
}

Symbol::Symbol(std::string p_name, MyTokenType p_type, SymbolKind p_kind) : name(p_name), type(p_type), kind(p_kind), is_set(false)
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
	is_set = true;
}

bool Symbol::IsSet()
{
	return is_set;
}

float Symbol::GetValue()
{
    return value;
}