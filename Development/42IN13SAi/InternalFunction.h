#pragma once

#include <string>

#include "Compiler.h"
#include "Parser.h"

class InternalFunction
{
public:
	InternalFunction(Compiler* compiler);
	virtual ~InternalFunction();
	CompilerNode GetInternalFunction(TokenType type);

private:
	Compiler*					compiler;
	CompilerNode				getCompilerNode(std::string function, int params);
	std::vector<CompilerNode*>	parseParameters(int expectedParams);
};
