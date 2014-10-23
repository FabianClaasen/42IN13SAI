#pragma once
#include "SymbolTable.h"
#include "SubroutineTable.h"
#include "CompilerNode.h"

class VirtualMachine
{
public:
	VirtualMachine(SymbolTable symboltable, SubroutineTable subroutine, std::list<CompilerNode> compiler_nodes);
	virtual ~VirtualMachine();

	void ExecuteCode();

private:
	SymbolTable _symboltable;
	SubroutineTable _subroutine;
	std::list<CompilerNode> _compilernodes;

	
};

