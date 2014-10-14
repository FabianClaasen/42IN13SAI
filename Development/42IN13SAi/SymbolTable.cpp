#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
}

bool SymbolTable::HasSymbol(std::string symbolName)
{
    auto symbol = symbols.find(symbolName);
    if (symbol == symbols.end())
        return false;
    return true;
}

void SymbolTable::AddSymbol(Symbol symbol)
{
    if (!HasSymbol(symbol.name))
    {
        symbols.insert(std::make_pair(symbol.name, symbol));
    }
}

Symbol* SymbolTable::GetSymbol(std::string symbolName)
{
    if (HasSymbol(symbolName))
        return &(symbols[symbolName]);
    return nullptr;
}

SymbolTable::~SymbolTable()
{
    
}