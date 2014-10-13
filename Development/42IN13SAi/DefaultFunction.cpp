#include "DefaultFunction.h"

DefaultFunction::DefaultFunction()
{
}


DefaultFunction::~DefaultFunction()
{
}

CompilerNode DefaultFunction::Printline()
{
	std::vector<CompilerNode> compiler_nodes = parseParameters(1);
	return CompilerNode("$prnt",compiler_nodes,nullptr);
}

CompilerNode DefaultFunction::Stop()
{
	std::vector<CompilerNode> compiler_nodes = parseParameters(0);
	return CompilerNode("$stop", compiler_nodes, nullptr);
}
