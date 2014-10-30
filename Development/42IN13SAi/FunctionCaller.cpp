//
//  FunctionCaller.cpp
//  mac_42IN13SAI
//
//  Copyright (c) 2014 42IN13SAI. All rights reserved.
//

#include "FunctionCaller.h"
#include "VirtualMachine.h"

FunctionCaller::FunctionCaller() {}
FunctionCaller::FunctionCaller(VirtualMachine* virtualMachine)
{
	fillFunctionMap();
    this->virtualMachine = virtualMachine;
}

CompilerNode* FunctionCaller::Call(const std::string& functionName, CompilerNode compilerNode)
{
    MFP functionPointer = functions[functionName];
    return (virtualMachine->*functionPointer)(compilerNode);
}

void FunctionCaller::fillFunctionMap()
{
	functions.insert(functionMap::value_type("$assignment", &VirtualMachine::ExecuteAssignment));
    
    // Simple Math Operations
	functions.insert(functionMap::value_type("$add", &VirtualMachine::ExecuteAddOperation));
	functions.insert(functionMap::value_type("$min", &VirtualMachine::ExecuteMinusOperation));
    functions.insert(functionMap::value_type("$mul", &VirtualMachine::ExecuteMinusOperation));
    functions.insert(functionMap::value_type("$div", &VirtualMachine::ExecuteDivideOperation));
    
    // Complex Math Operations
    functions.insert(functionMap::value_type("$sin", &VirtualMachine::ExecuteSinOperation));
    functions.insert(functionMap::value_type("$cos", &VirtualMachine::ExecuteCosOperation));
    functions.insert(functionMap::value_type("$tan", &VirtualMachine::ExecuteTanOperation));
}

FunctionCaller::~FunctionCaller()
{
    delete virtualMachine;
    virtualMachine = nullptr;
}