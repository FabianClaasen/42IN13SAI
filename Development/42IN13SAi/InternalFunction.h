#pragma once

#include <string>
#include "Compiler.h"

class DefaultFunction;
class MathFunction;
class PhysicsFunction;

class InternalFunction : public Compiler 
{
public:
	InternalFunction();
	virtual ~InternalFunction();
	CompilerNode GetInternalFunction(Token id);

private:
	DefaultFunction*			defaultFunction;
	MathFunction*				mathFunction;
	PhysicsFunction*			physicsFunction;
	CompilerNode				getFunctionNode(std::string function, int params);
	std::vector<CompilerNode>	parseParameters(int expectedParams);
};
