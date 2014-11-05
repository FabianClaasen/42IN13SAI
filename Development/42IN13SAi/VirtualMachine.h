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
    CompilerNode* ExecuteNodes(std::vector<CompilerNode> nodes);
    CompilerNode* ExecuteNodes(std::vector<CompilerNode> nodes, int currentIndex);

    // Function Operations
    CompilerNode* ExecuteFunction(CompilerNode compilerNode);
    CompilerNode* ExecuteReturn(CompilerNode compilerNode);
    
    // Variable operations
	CompilerNode* ExecuteAssignment(CompilerNode compilerNode);
    CompilerNode* ExecuteGetVariable(CompilerNode compilerNode);

	//Default operations
	CompilerNode* ExecutePrint(CompilerNode compilerNode);
	CompilerNode* ExecuteStop(CompilerNode compilerNode);
    
    // Loop operations
    CompilerNode* ExecuteWhile(CompilerNode compilerNode);

    // Simple math operations
    CompilerNode* ExecuteAddOperation(CompilerNode compilerNode);
	CompilerNode* ExecuteMinusOperation(CompilerNode compilerNode);
    CompilerNode* ExecuteMultiplyOperation(CompilerNode compilerNode);
    CompilerNode* ExecuteDivideOperation(CompilerNode compilerNode);
	CompilerNode* ExecuteModuloOperation(CompilerNode compilerNode);
    CompilerNode* ExecuteUniPlusOperation(CompilerNode compilerNode);
    CompilerNode* ExecuteUniMinOperation(CompilerNode compilerNode);
    
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
	SymbolTable* mainSymboltable;
    SymbolTable* subSymbolTable;
	SubroutineTable* subroutineTable;
    Subroutine* subSubroutine;
    CompilerNode PeekNext(int currentIndex, std::vector<CompilerNode> nodes);
    CompilerNode PeekPrevious(int currentIndex, std::vector<CompilerNode> nodes);
	CompilerNode GetNext(int* currentIndex, std::vector<CompilerNode> nodes);
    CompilerNode* CallFunction(CompilerNode node);
	FunctionCaller* function_caller;

	std::vector<CompilerNode> _compilernodes;
	std::vector<CompilerNode> _received_compilernodes;
	int currentIndex = -1;

	//bool isAction(MFP);
};

