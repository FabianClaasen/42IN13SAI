#include "SymbolTable.h"

SymbolTable::SymbolTable()
{
    symbols = std::make_shared<symbolMap>(symbolMap());
}

std::vector<std::pair<std::string, Symbol>>::iterator SymbolTable::HasSymbolIt(std::string symbolName)
{
    symbolMap::iterator it = std::find_if(symbols->begin(), symbols->end(),
                                          [&](std::pair<std::string, Symbol>& pair){
                                              return pair.first == symbolName;
                                          });
    return it;
}

bool SymbolTable::HasSymbol(std::string symbolName)
{
	if (symbols->size() > 0)
	{
        return HasSymbolIt(symbolName) != symbols->end();
	}
    
    return false;
}

void SymbolTable::AddSymbol(Symbol symbol)
{
    if (!HasSymbol(symbol.name))
    {
        symbols->push_back(std::make_pair(symbol.name, symbol));
    }
}

Symbol* SymbolTable::GetSymbol(std::string symbolName)
{
    symbolMap::iterator it = HasSymbolIt(symbolName);
    if (it != symbols->end())
        return &(it->second);
    return nullptr;
}

std::vector<Symbol*> SymbolTable::GetSymbolVector()
{
    std::vector<Symbol*> vSymbols;
    for( symbolMap::iterator it = symbols->begin(); it != symbols->end(); ++it ) {
        if (it->second.kind == SymbolKind::Parameter)
            vSymbols.push_back( &it->second );
    }
    return vSymbols;
}

int SymbolTable::Size()
{
    return (int)symbols->size();
}

int SymbolTable::ParameterSize()
{
    int parameterCount = 0;
    
    for( symbolMap::iterator it = symbols->begin(); it != symbols->end(); ++it ) {
        if (it->second.kind == SymbolKind::Parameter)
            parameterCount++;
    }
    return parameterCount;
}

SymbolTable::~SymbolTable()
{
    
}