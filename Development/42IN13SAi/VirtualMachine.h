#pragma once
#include <stdlib.h>
#include <math.h>
#include <cmath>

#include "SymbolTable.h"
#include "SubroutineTable.h"
#include "CompilerNode.h"
#include "FunctionCaller.h"
#include "ParameterException.h"

class VirtualMachine
{
public:
	VirtualMachine(SymbolTable* symboltable, SubroutineTable* subroutine, std::vector<CompilerNode> compiler_nodes);
	virtual ~VirtualMachine();

	void ExecuteCode();

    // Variable operations
	CompilerNode* ExecuteAssignment(CompilerNode compilerNode);
    CompilerNode* ExecuteGetVariable(CompilerNode compilerNode);

	//Default operations
	CompilerNode* ExecutePrint(CompilerNode compilerNode);
	CompilerNode* ExecuteStop(CompilerNode compilerNode);

    // Simple math operations
    CompilerNode* ExecuteAddOperation(CompilerNode compilerNode);
	CompilerNode* ExecuteMinusOperation(CompilerNode compilerNode);
    CompilerNode* ExecuteMultiplyOperation(CompilerNode compilerNode);
    CompilerNode* ExecuteDivideOperation(CompilerNode compilerNode);
	CompilerNode* ExecuteModuloOperation(CompilerNode compilerNode);
    
	//Conditional statements
	CompilerNode* ExecuteLessCondition(CompilerNode compilerNode);
	CompilerNode* ExecuteGreaterCondition(CompilerNode compilerNode);
	CompilerNode* ExecuteEqualCondition(CompilerNode compilerNode);
	CompilerNode* ExecuteNotEqualCondition(CompilerNode compilerNode);

    // Complexer math operations
    CompilerNode* ExecuteSinOperation(CompilerNode compilerNode);
    CompilerNode* ExecuteCosOperation(CompilerNode compilerNode);
    CompilerNode* ExecuteTanOperation(CompilerNode compilerNode);
	
private:
	SymbolTable* _symboltable;
	SubroutineTable* _subroutine;
	CompilerNode* PeekNext();
	CompilerNode GetNext();
    CompilerNode* CallFunction(CompilerNode node);
	FunctionCaller* function_caller;

	std::vector<CompilerNode> _compilernodes;
	std::vector<CompilerNode> _received_compilernodes;
	int currentIndex = -1;

	//bool isAction(MFP);
};

