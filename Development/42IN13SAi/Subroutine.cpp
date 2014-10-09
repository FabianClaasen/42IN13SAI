//
//  Subroutine.cpp
//  mac_42IN13SAI
//
//  Copyright (c) 2014 42IN13SAI. All rights reserved.
//

#include "Subroutine.h"

Subroutine::Subroutine() :name(""), returnType(ReturnKind::None), kind(SubroutineKind::None), parameters(symbolMap())
{
    
}

Subroutine::Subroutine(std::string p_name, ReturnKind p_return, SubroutineKind p_kind, symbolMap p_parameters) :name(p_name), returnType(p_return), kind(p_kind), parameters(p_parameters)
{
    
}

bool Subroutine::HasLocal(std::string name)
{
    auto symbol = locals.find(name);
    if (symbol == locals.end())
        return false;
    return true;
}

void Subroutine::AddLocal(Symbol symbol)
{
    if (HasLocal(symbol.name))
    {
        locals.insert(std::make_pair(symbol.name, symbol));
    }
}

Symbol* Subroutine::GetLocal(std::string name)
{
    if (HasLocal(name))
    {
        return &(locals[name]);
    }
    return nullptr;
}

Subroutine::~Subroutine()
{
    
}