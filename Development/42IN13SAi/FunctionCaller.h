//
//  FunctionCaller.h
//  mac_42IN13SAI
//
//  Copyright (c) 2014 42IN13SAI. All rights reserved.
//

#pragma once

#include <map>
#include <string>
#include "CompilerNode.h"
class VirtualMachine;

class FunctionCaller {
private:
    VirtualMachine* virtualMachine;
    
    typedef CompilerNode* (VirtualMachine::*MFP)(CompilerNode);
    typedef std::map<std::string, MFP> functionMap;
    functionMap functions;
    
    void fillFunctionMap();
    
public:
	FunctionCaller();
    FunctionCaller(VirtualMachine* virtualMachine);
    virtual ~FunctionCaller();
	CompilerNode compilerNode;
    CompilerNode* Call(const std::string& functionName, CompilerNode compilerNode);
};