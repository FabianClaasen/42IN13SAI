#pragma once

#include <string>

#include "Compiler.h"
#include "Parser.h"
class Constant
{
public:
	Constant(Compiler* compiler);
	virtual ~Constant();
	std::shared_ptr<CompilerNode> GetConstant(MyTokenType type);


private:
	Compiler* compiler;
	std::shared_ptr<CompilerNode> getCompilerNode(std::string constant);
};