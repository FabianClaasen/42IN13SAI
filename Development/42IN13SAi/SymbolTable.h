#pragma once

#include <unordered_map>
#include <string>
#include <algorithm>
#include <vector>
#include <memory>
#include "Symbol.h"
#include <memory>

class SymbolTable {
private:
    typedef std::vector<std::pair<std::string, Symbol>> symbolMap;
    std::shared_ptr<symbolMap> symbols;

public:
    SymbolTable();
    virtual ~SymbolTable();
    
    symbolMap::iterator HasSymbolIt(std::string symbolName);
    bool HasSymbol(std::string symbolName);
    void AddSymbol(Symbol symbol);
    Symbol* GetSymbol(std::string symbolName);
    std::vector<Symbol*> GetSymbolVector();
    int Size();
    int ParameterSize();
};
