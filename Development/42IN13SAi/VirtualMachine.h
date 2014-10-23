#pragma once
#include "SymbolTable.h"
#include "SubroutineTable.h"
#include "CompilerNode.h"
#include "FunctionCaller.h"

class VirtualMachine
{
public:
	VirtualMachine(SymbolTable symboltable, SubroutineTable subroutine, std::vector<CompilerNode> compiler_nodes);
	virtual ~VirtualMachine();

	void ExecuteCode();

private:
	SymbolTable _symboltable;
	SubroutineTable _subroutine;
	CompilerNode* PeekNext();
	CompilerNode GetNext();
	FunctionCaller* function_caller;

	std::vector<CompilerNode> _compilernodes;
	std::vector<CompilerNode> received_compilernodes;

	int currentIndex = -1;
	//typedef CompilerNode(VirtualMachine::*MFP)(CompilerNode);

	//bool isAction(MFP);
};

