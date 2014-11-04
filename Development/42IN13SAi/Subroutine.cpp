//
//  Subroutine.cpp
//  mac_42IN13SAI
//
//  Copyright (c) 2014 42IN13SAI. All rights reserved.
//

#include "Subroutine.h"

Subroutine::Subroutine() :name(""), returnType(MyTokenType::None), kind(SubroutineKind::None), isEmpty(true)
{
}

Subroutine::Subroutine(std::string p_name, MyTokenType p_return, SubroutineKind p_kind, SymbolTable p_parameters) : name(p_name), returnType(p_return), kind(p_kind), symbolTable(p_parameters), isEmpty(false)
{
}

Subroutine& Subroutine::operator=(const Subroutine& other)
{
	if (this != &other)
	{
		kind = other.kind;
		returnType = other.returnType;
		name = other.name;
		isEmpty = other.isEmpty;
		subroutineCompilerNodes = other.subroutineCompilerNodes;
		symbolTable = other.symbolTable;
	}
	return *this;
}

Subroutine::~Subroutine()
{
}

bool Subroutine::HasLocal(std::string name)
{
	return symbolTable.HasSymbol(name);
}

void Subroutine::AddLocal(Symbol symbol)
{
    if (HasLocal(symbol.name))
    {
		symbolTable.AddSymbol(symbol);
    }
}

SymbolTable* Subroutine::GetSymbolTable()
{
    return &symbolTable;
}

Symbol* Subroutine::GetLocal(std::string name)
{
    if (HasLocal(name))
    {
        return symbolTable.GetSymbol(name);
    }
    return nullptr;
}

void Subroutine::AddCompilerNode(CompilerNode node)
{
	subroutineCompilerNodes.push_back(node);
}

std::list<CompilerNode>* Subroutine::GetCompilerNodeCollection()
{
    return &subroutineCompilerNodes;
}

std::vector<CompilerNode>* Subroutine::GetCompilerNodeVector()
{
    std::vector<CompilerNode>* nodeVector = new std::vector<CompilerNode>{ std::begin(subroutineCompilerNodes), std::end(subroutineCompilerNodes) };
	return nodeVector;
}