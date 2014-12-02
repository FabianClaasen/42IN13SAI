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

std::vector<Symbol*> SymbolTable::GetSymbolVector()
{
    std::vector<Symbol*> vSymbols;
    for( symbolMap::iterator it = symbols.begin(); it != symbols.end(); ++it ) {
        if (it->second.kind == SymbolKind::Parameter)
            vSymbols.push_back( &it->second );
    }
    return vSymbols;
}

int SymbolTable::Size()
{
    return (int)symbols.size();
}

int SymbolTable::ParameterSize()
{
    int parameterCount = 0;
    
    for( symbolMap::iterator it = symbols.begin(); it != symbols.end(); ++it ) {
        if (it->second.kind == SymbolKind::Parameter)
            parameterCount++;
    }
    return parameterCount;
}

SymbolTable::~SymbolTable()
{
    
}