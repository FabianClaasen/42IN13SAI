#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include "Symbol.h"

class SymbolTable {
private:
    typedef std::unordered_map<std::string, Symbol> symbolMap;
    symbolMap symbols;

public:
    SymbolTable();
    virtual ~SymbolTable();
    
    bool HasSymbol(std::string symbolName);
    void AddSymbol(Symbol symbol);
    Symbol* GetSymbol(std::string symbolName);
    std::vector<Symbol*> GetSymbolVector();
    int Size();
    int ParameterSize();
};