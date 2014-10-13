//
//  Subroutine.h
//  mac_42IN13SAI
//
//  Copyright (c) 2014 42IN13SAI. All rights reserved.
//

#pragma once
#include <string>
#include <unordered_map>
#include "Symbol.h"
#include "SymbolTable.h"
#include "SubroutineKind.h"
#include "ReturnKind.h"

class Subroutine {
private:
	SymbolTable symbolTable;

public:	
    const SubroutineKind kind;
    const ReturnKind returnType;
    const std::string name;
    
    Subroutine();
	Subroutine(std::string p_name, ReturnKind p_return, SubroutineKind p_kind, SymbolTable p_parameters);
    virtual ~Subroutine();
    
    void AddLocal(Symbol symbol);
    bool HasLocal(std::string name);
    Symbol* GetLocal(std::string name);
};


