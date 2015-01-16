#pragma once

#include <string>
#include "MyTokenType.h"
#include "SymbolKind.h"

class Symbol
{
private:
	long double value;
public:
    const std::string name;
    const MyTokenType type;
    const SymbolKind kind;
	bool is_set = false;

    Symbol();
    Symbol(std::string p_name, MyTokenType p_type, SymbolKind p_kind);
    Symbol& operator=(const Symbol& other);
    virtual ~Symbol();
    
	long double GetValue();
	bool IsSet();
	void SetValue(long double p_value);
};