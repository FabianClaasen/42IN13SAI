#pragma once

#include <string>

#include "Compiler.h"
#include "Parser.h"

class InternalFunction
{
public:
	InternalFunction(Compiler* compiler);
	virtual ~InternalFunction();
	std::shared_ptr<CompilerNode> GetInternalFunction(MyTokenType type);

private:
	Compiler* compiler;
	std::shared_ptr<CompilerNode> getCompilerNode(std::string function, int params);
	std::vector<std::shared_ptr<CompilerNode>> parseParameters(int expectedParams);
};
