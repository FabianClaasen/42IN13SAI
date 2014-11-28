//
//  Subroutine.cpp
//  mac_42IN13SAI
//
//  Copyright (c) 2014 42IN13SAI. All rights reserved.
//

#include "Subroutine.h"

Subroutine::Subroutine() :name(""), returnType(MyTokenType::None), kind(SubroutineKind::None), isEmpty(true)
{
	subroutineCompilerNodes = std::make_shared<LinkedList>();
}

Subroutine::Subroutine(std::string p_name, MyTokenType p_return, SubroutineKind p_kind, SymbolTable p_parameters) : name(p_name), returnType(p_return), kind(p_kind), symbolTable(p_parameters), isEmpty(false)
{
	subroutineCompilerNodes = std::make_shared<LinkedList>();
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
	if (!HasLocal(symbol.name))
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

void Subroutine::AddCompilerNode(std::shared_ptr<CompilerNode> node)
{
	subroutineCompilerNodes->PushBack(node);
}

std::shared_ptr<LinkedList> Subroutine::GetCompilerNodeCollection()
{
	return subroutineCompilerNodes;
}

std::vector<std::shared_ptr<CompilerNode>> Subroutine::GetCompilerNodeVector()
{
	std::vector<std::shared_ptr<CompilerNode>> nodeVector;

	for (ListIterator it = subroutineCompilerNodes->begin(); it == subroutineCompilerNodes->end(); ++it)
	{
		nodeVector.push_back(*it);
	}

	return nodeVector;
}