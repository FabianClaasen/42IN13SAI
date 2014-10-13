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
	Match(TokenType::EOL);
	return CompilerNode("$prnt",compiler_nodes,nullptr);
}

CompilerNode DefaultFunction::Stop()
{
	std::vector<CompilerNode> compiler_nodes = parseParameters(1);
	//CheckNumberOfParameters(compiler_nodes, 0);
	Match(TokenType::EOL);
	return CompilerNode("$stop", compiler_nodes, nullptr);
}
