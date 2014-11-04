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
    SubroutineKind kind;
    MyTokenType returnType;
    std::string name;
	bool isEmpty;
    
    Subroutine();
	Subroutine(std::string p_name, MyTokenType p_return, SubroutineKind p_kind, SymbolTable p_parameters);
	Subroutine& operator=(const Subroutine& other);
    virtual ~Subroutine();
    
    void AddLocal(Symbol symbol);
    bool HasLocal(std::string name);
    Symbol* GetLocal(std::string name);
    SymbolTable* GetSymbolTable();

	std::list<CompilerNode>* GetCompilerNodeCollection();
    std::vector<CompilerNode>* GetCompilerNodeVector();
	void AddCompilerNode(CompilerNode node);
};


