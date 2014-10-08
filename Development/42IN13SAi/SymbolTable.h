//
//  SymbolTable.h
//  mac_42IN13SAI
//
//  Copyright (c) 2014 42IN13SAI. All rights reserved.
//

#include <unordered_map>
#include <string>
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
};