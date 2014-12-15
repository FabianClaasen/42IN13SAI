#pragma once
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <memory>
#include <QThread>
#include <QObject>

#include "SymbolTable.h"
#include "SubroutineTable.h"
#include "CompilerNode.h"
#include "FunctionCaller.h"
#include "LinkedList.h"

#include "ParameterException.h"
#include "MissingCompilerNodeException.h"
#include "MissingMainFunctionException.h"
#include "FunctionNameExpectedException.h"
#include "SubroutineNotFoundException.h"
#include "UnknownExpressionException.h"
#include "InvalidInputException.h"
#include "ZeroDivideException.h"
#include "UnexpectedTypeException.h"

class VirtualMachine : public QThread
{	
	Q_OBJECT

public:
	VirtualMachine(SymbolTable* symboltable, SubroutineTable* subroutine, std::shared_ptr<LinkedList> compiler_nodes);
	VirtualMachine(const VirtualMachine &other);
	VirtualMachine& operator=(const VirtualMachine &other);
	virtual ~VirtualMachine();

	void ExecuteCode();
	std::shared_ptr<CompilerNode> ExecuteNodes(std::shared_ptr<LinkedList> nodes);
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
	std::shared_ptr<CompilerNode> ExecuteFor(CompilerNode compilerNode);

	// Condition operations
	std::shared_ptr<CompilerNode> ExecuteIf(CompilerNode compilerNode);

	// Simple math operations
	std::shared_ptr<CompilerNode> ExecuteAddOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteMinusOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteMultiplyOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteDivideOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteUniPlusOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteUniMinOperation(CompilerNode compilerNode);
	
    // Condition add statements
    std::shared_ptr<CompilerNode> ExecuteAnd(CompilerNode compilerNode);
    std::shared_ptr<CompilerNode> ExecuteOr(CompilerNode compilerNode);
    
	//Conditional statements
	std::shared_ptr<CompilerNode> ExecuteLessCondition(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteLessOrEqCondition(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteGreaterCondition(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteGreaterOrEqCondition(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteEqualCondition(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteNotEqualCondition(CompilerNode compilerNode);

	// Complexer math operations
	std::shared_ptr<CompilerNode> ExecuteSqrOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteCbcOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecutePowOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteSqrtOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteCbrtOperation(CompilerNode compilerNode);

	std::shared_ptr<CompilerNode> ExecuteSinOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteCosOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteTanOperation(CompilerNode compilerNode);

	std::shared_ptr<CompilerNode> ExecuteDegreeOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteRadiantOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecutePercentOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecutePermillageOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteCommonLogOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteBinaryLogOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteNaturalLogOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteLogOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteModuloOperation(CompilerNode compilerNode);

	std::shared_ptr<CompilerNode> ExecuteGcdOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteDiscriminantOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteAbcOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteFibonacciOperation(CompilerNode compilerNode);

	std::shared_ptr<CompilerNode> ExecuteDistanceOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteVelocityOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteTimeOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteAverageVelocityOperation(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteAccelerationOperation(CompilerNode compilerNode);
	
	std::shared_ptr<CompilerNode> ExecutePiConstant(CompilerNode compilerNode);
	std::shared_ptr<CompilerNode> ExecuteEConstant(CompilerNode compilerNode);

	void run();
	void quit();
	void finished();
	void exit();

private:
	SymbolTable* globalsSymboltable; // Globals symboltable
	SymbolTable* currentSymbolTable; // User function symboltable

	SubroutineTable* subroutineTable; // Main subroutineTable
	Subroutine* currentSubroutine; // User function subroutine table

	// Node methods
	std::shared_ptr<CompilerNode> GetNext(std::shared_ptr<LinkedList> nodes);

	// Function call methods
	std::shared_ptr<CompilerNode> CallFunction(CompilerNode node);
	std::unique_ptr<FunctionCaller> function_caller;

	// Check and return parameters
	std::vector<std::shared_ptr<CompilerNode>> CheckParameters(CompilerNode &compilerNode, int amount);

	// Nodes 
	std::shared_ptr<LinkedList> globalsList;
	std::map<std::string, std::shared_ptr<LinkedList>> nodeLists;

	bool is_running;
	
signals:
	void PrintException(QString);
	void PrintOutput(QString);
	void Finished();

};