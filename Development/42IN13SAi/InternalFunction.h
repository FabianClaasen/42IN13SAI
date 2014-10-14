#pragma once

#include <string>
#include "Compiler.h"

class InternalFunction : public Compiler 
{
public:
	InternalFunction();
	virtual ~InternalFunction();
	CompilerNode GetInternalFunction(Token id);

private:
	CompilerNode				getCompilerNode(std::string function, int params);
	std::vector<CompilerNode>	parseParameters(int expectedParams);
};
