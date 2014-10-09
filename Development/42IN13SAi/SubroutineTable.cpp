//
//  SubroutineTable.cpp
//  mac_42IN13SAI
//
//  Copyright (c) 2014 42IN13SAI. All rights reserved.
//

#include "SubroutineTable.h"

SubroutineTable::SubroutineTable()
{
}

SubroutineTable::~SubroutineTable()
{
}

void SubroutineTable::AddSubroutine(Subroutine subroutine)
{
    if (HasSubroutine(subroutine.name)) {
        subroutines.insert(std::make_pair(subroutine.name, subroutine));
    }
}

bool SubroutineTable::HasSubroutine(std::string name)
{
    auto subroutine = subroutines.find(name);
    if (subroutine != subroutines.end())
        return true;
    return false;
}

Subroutine* SubroutineTable::GetSubroutine(std::string name)
{
    if (HasSubroutine(name))
    {
        return &(subroutines[name]);
    }
    return nullptr;
}

