#pragma once
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <memory>

#include "SymbolTable.h"
#include "SubroutineTable.h"
#include "CompilerNode.h"
#include "FunctionCaller.h"
#include "ParameterException.h"

class VirtualMachine
{
    typedef std::list<std::shared_ptr<CompilerNode>> compilerNodeList;
    
public:
	VirtualMachine(SymbolTable* symboltable, SubroutineTable* subroutine, compilerNodeList compiler_nodes);
    VirtualMachine(const VirtualMachine &other);
    VirtualMachine& operator=(const VirtualMachine &other);
	virtual ~VirtualMachine();

	void ExecuteCode();
    std::shared_ptr<CompilerNode> ExecuteNodes(std::list<std::shared_ptr<CompilerNode>> nodes);
    std::shared_ptr<CompilerNode> ExecuteNodes(std::list<std::shared_ptr<CompilerNode>> nodes, int currentIndex);

    // Function Operations
    std::shared_ptr<CompilerNode> ExecuteFunction(CompilerNode compilerNode);
    std::shared_ptr<CompilerNode> ExecuteReturn(CompilerNode compilerNode);
    
    // Variable operations
	std::shared_ptr<CompilerNode> ExecuteAssignment(CompilerNode compilerNode);
    std::shared_ptr<CompilerNode> ExecuteGetVariable(CompilerNode compilerNode);

	//Default operations
	std::shared_ptr<CompilerNode> ExecutePrint(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteStop(CompilerNode compilerNode);
    
    // Loop operations
    std::shared_ptr<CompilerNode> ExecuteWhile(CompilerNode compilerNode);

    // Simple math operations
    std::shared_ptr<CompilerNode> ExecuteAddOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteMinusOperation(CompilerNode compilerNode);
    std::shared_ptr<CompilerNode> ExecuteMultiplyOperation(CompilerNode compilerNode);
    std::shared_ptr<CompilerNode> ExecuteDivideOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteModuloOperation(CompilerNode compilerNode);
    std::shared_ptr<CompilerNode> ExecuteUniPlusOperation(CompilerNode compilerNode);
    std::shared_ptr<CompilerNode> ExecuteUniMinOperation(CompilerNode compilerNode);
    
	//Conditional statements
	std::shared_ptr<CompilerNode> ExecuteLessCondition(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteGreaterCondition(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteEqualCondition(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteNotEqualCondition(CompilerNode compilerNode);

    // Complexer math operations
    std::shared_ptr<CompilerNode> ExecuteSinOperation(CompilerNode compilerNode);
    std::shared_ptr<CompilerNode> ExecuteCosOperation(CompilerNode compilerNode);
    std::shared_ptr<CompilerNode> ExecuteTanOperation(CompilerNode compilerNode);
	
private:
	SymbolTable* mainSymboltable;
    SymbolTable* subSymbolTable;
	SubroutineTable* subroutineTable;
    Subroutine* subSubroutine;
    CompilerNode PeekNext(std::string functionName, compilerNodeList nodes);
	CompilerNode PeekPrevious(std::string functionName, compilerNodeList nodes);
	CompilerNode GetNext(std::string functionName, compilerNodeList &nodes);
    std::shared_ptr<CompilerNode> CallFunction(CompilerNode node);
    std::unique_ptr<FunctionCaller> function_caller;

    std::map<std::string, compilerNodeList::iterator> nodeIterators;
	std::list<std::shared_ptr<CompilerNode>> compilerNodes;
	std::list<std::shared_ptr<CompilerNode>> receivedCompilerNodes;
    std::shared_ptr<int> currentIndex;

	//bool isAction(MFP);
};

