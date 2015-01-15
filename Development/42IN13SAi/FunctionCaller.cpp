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

std::shared_ptr<CompilerNode> FunctionCaller::Call(const std::string& functionName, CompilerNode compilerNode)
{
	MFP functionPointer = functions[functionName];
	return (virtualMachine->*functionPointer)(compilerNode);
}

void FunctionCaller::fillFunctionMap()
{
	// Function Operations
	functions.insert(functionMap::value_type("$functionCall", &VirtualMachine::ExecuteFunction));
	functions.insert(functionMap::value_type("$ret", &VirtualMachine::ExecuteReturn));
	
	// Variable Operations
	functions.insert(functionMap::value_type("$assignment", &VirtualMachine::ExecuteAssignment));
	functions.insert(functionMap::value_type("$getVariable", &VirtualMachine::ExecuteGetVariable));
	
	// Default operations
	functions.insert(functionMap::value_type("$prnt", &VirtualMachine::ExecutePrint));
	functions.insert(functionMap::value_type("$stop", &VirtualMachine::ExecuteStop));

	// Loop operations
	functions.insert(functionMap::value_type("$whileLoop", &VirtualMachine::ExecuteWhile));
	functions.insert(functionMap::value_type("$forLoop", &VirtualMachine::ExecuteFor));

	// Conditional operations
	functions.insert(functionMap::value_type("$if", &VirtualMachine::ExecuteIf));
	
	// Conditional statements
	functions.insert(functionMap::value_type("$less", &VirtualMachine::ExecuteLessCondition));
	functions.insert(functionMap::value_type("$lessOrEq", &VirtualMachine::ExecuteLessOrEqCondition));
	functions.insert(functionMap::value_type("$greater", &VirtualMachine::ExecuteGreaterCondition));
	functions.insert(functionMap::value_type("$greaterOrEq", &VirtualMachine::ExecuteGreaterOrEqCondition));
	functions.insert(functionMap::value_type("$equals", &VirtualMachine::ExecuteEqualCondition));
	functions.insert(functionMap::value_type("$notEquals", &VirtualMachine::ExecuteNotEqualCondition));
    functions.insert(functionMap::value_type("$and", &VirtualMachine::ExecuteAnd));
    functions.insert(functionMap::value_type("$or", &VirtualMachine::ExecuteOr));
    
	// Simple Math Operations
	functions.insert(functionMap::value_type("$add", &VirtualMachine::ExecuteAddOperation));
	functions.insert(functionMap::value_type("$min", &VirtualMachine::ExecuteMinusOperation));
	functions.insert(functionMap::value_type("$mul", &VirtualMachine::ExecuteMultiplyOperation));
	functions.insert(functionMap::value_type("$div", &VirtualMachine::ExecuteDivideOperation));
	functions.insert(functionMap::value_type("$uniMin", &VirtualMachine::ExecuteUniMinOperation));
	functions.insert(functionMap::value_type("$uniPlus", &VirtualMachine::ExecuteUniPlusOperation));

	// Complex Math Operations
	functions.insert(functionMap::value_type("$sin", &VirtualMachine::ExecuteSinOperation));
	functions.insert(functionMap::value_type("$sinr", &VirtualMachine::ExecuteSinrOperation));
	functions.insert(functionMap::value_type("$cos", &VirtualMachine::ExecuteCosOperation));
	functions.insert(functionMap::value_type("$cosr", &VirtualMachine::ExecuteCosrOperation));
	functions.insert(functionMap::value_type("$tan", &VirtualMachine::ExecuteTanOperation));
	functions.insert(functionMap::value_type("$tanr", &VirtualMachine::ExecuteTanrOperation));

	functions.insert(functionMap::value_type("$sqr", &VirtualMachine::ExecuteSqrOperation));
	functions.insert(functionMap::value_type("$cbc", &VirtualMachine::ExecuteCbcOperation));
	functions.insert(functionMap::value_type("$pow", &VirtualMachine::ExecutePowOperation));
	functions.insert(functionMap::value_type("$sqrt", &VirtualMachine::ExecuteSqrtOperation));
	functions.insert(functionMap::value_type("$cbrt", &VirtualMachine::ExecuteCbrtOperation));

	functions.insert(functionMap::value_type("$rad", &VirtualMachine::ExecuteRadiantOperation));
	functions.insert(functionMap::value_type("$deg", &VirtualMachine::ExecuteDegreeOperation));

	functions.insert(functionMap::value_type("$perc", &VirtualMachine::ExecutePercentOperation));
	functions.insert(functionMap::value_type("$perm", &VirtualMachine::ExecutePermillageOperation));

	functions.insert(functionMap::value_type("$log10", &VirtualMachine::ExecuteCommonLogOperation));
	functions.insert(functionMap::value_type("$log2", &VirtualMachine::ExecuteBinaryLogOperation));
	functions.insert(functionMap::value_type("$ln", &VirtualMachine::ExecuteNaturalLogOperation));
	functions.insert(functionMap::value_type("$log", &VirtualMachine::ExecuteLogOperation));

	functions.insert(functionMap::value_type("$mod", &VirtualMachine::ExecuteModuloOperation));

	functions.insert(functionMap::value_type("$gcd", &VirtualMachine::ExecuteGcdOperation));
	functions.insert(functionMap::value_type("$dis", &VirtualMachine::ExecuteDiscriminantOperation));
	functions.insert(functionMap::value_type("$abc", &VirtualMachine::ExecuteAbcOperation)); 
	functions.insert(functionMap::value_type("$fib", &VirtualMachine::ExecuteFibonacciOperation));
	functions.insert(functionMap::value_type("$pyt", &VirtualMachine::ExecutePythagoreanOperation));
	functions.insert(functionMap::value_type("$pyth", &VirtualMachine::ExecutePythagoreanHOperation));
	

	// Physics functions
	functions.insert(functionMap::value_type("$dist", &VirtualMachine::ExecuteDistanceOperation));
	functions.insert(functionMap::value_type("$velo", &VirtualMachine::ExecuteVelocityOperation));
	functions.insert(functionMap::value_type("$time", &VirtualMachine::ExecuteTimeOperation));
	functions.insert(functionMap::value_type("$avgv", &VirtualMachine::ExecuteAverageVelocityOperation));
	functions.insert(functionMap::value_type("$accl", &VirtualMachine::ExecuteAccelerationOperation));

	// Math Constants
	functions.insert(functionMap::value_type("$mathpi", &VirtualMachine::ExecutePiConstant));
	functions.insert(functionMap::value_type("$mathe", &VirtualMachine::ExecuteEConstant));


}

FunctionCaller::~FunctionCaller()
{
}