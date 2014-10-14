//
//  Subroutine.cpp
//  mac_42IN13SAI
//
//  Copyright (c) 2014 42IN13SAI. All rights reserved.
//

#include "Subroutine.h"

Subroutine::Subroutine() :name(""), returnType(TokenType::None), kind(SubroutineKind::None)
{
}

Subroutine::Subroutine(std::string p_name, TokenType p_return, SubroutineKind p_kind, SymbolTable p_parameters) : name(p_name), returnType(p_return), kind(p_kind), symbolTable(p_parameters)
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

Symbol* Subroutine::GetLocal(std::string name)
{
    if (HasLocal(name))
    {
        return symbolTable.GetSymbol(name);
    }
    return nullptr;
}

Subroutine::~Subroutine()
{
}