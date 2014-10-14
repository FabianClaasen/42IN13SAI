//
//  SubroutineTable.h
//  mac_42IN13SAI
//
//  Copyright (c) 2014 42IN13SAI. All rights reserved.
//

#pragma once

#include <string>
#include <unordered_map>
#include "Subroutine.h"

class SubroutineTable {
private:
    typedef std::unordered_map<std::string, Subroutine> subroutineMap;
    subroutineMap subroutines;
public:
    SubroutineTable();
    virtual ~SubroutineTable();
    
    void AddSubroutine(Subroutine subroutine);
    bool HasSubroutine(std::string name);
    Subroutine* GetSubroutine(std::string name);
};


