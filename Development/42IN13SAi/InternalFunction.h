#pragma once

#include <string>

#include "Compiler.h"

class InternalFunction
{
public:
	InternalFunction(Compiler* compiler);
	virtual ~InternalFunction();
	CompilerNode GetInternalFunction(Token id);

private:
	Compiler*					compiler;
	CompilerNode				getCompilerNode(std::string function, int params);
	std::vector<CompilerNode*>	parseParameters(int expectedParams);
};
