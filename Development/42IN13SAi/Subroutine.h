//
//  Subroutine.h
//  mac_42IN13SAI
//
//  Copyright (c) 2014 42IN13SAI. All rights reserved.
//

#pragma once

#include <string>
#include <unordered_map>
#include <list>
#include "Symbol.h"
#include "SymbolTable.h"
#include "SubroutineKind.h"
#include "CompilerNode.h"
#include "Token.h"

class Subroutine {
private:
	SymbolTable symbolTable;
	std::list<CompilerNode> subroutineCompilerNodes;

public:	
    const SubroutineKind kind;
    const TokenType returnType;
    const std::string name;
	const bool isEmpty;
    
    Subroutine();
	Subroutine(std::string p_name, TokenType p_return, SubroutineKind p_kind, SymbolTable p_parameters);
    virtual ~Subroutine();
    
    void AddLocal(Symbol symbol);
    bool HasLocal(std::string name);
    Symbol* GetLocal(std::string name);

	std::list<CompilerNode>* GetCompilerNodeCollection();
};


