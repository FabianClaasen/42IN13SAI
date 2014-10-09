#include "DefaultFunction.h"

DefaultFunction::DefaultFunction()
{
}


DefaultFunction::~DefaultFunction()
{
}

CompilerNode DefaultFunction::Printline()
{
	std::vector<CompilerNode> compiler_nodes = parseParameters();
	Match(TokenType::EOL);
	return CompilerNode(std::string("$prnt"),compiler_nodes,nullptr);
}
CompilerNode DefaultFunction::Stop()
{
	std::vector<CompilerNode> compiler_nodes = parseParameters();
	CheckNumberOfParameters(compiler_nodes, 0);
	Match(TokenType::EOL);
	return CompilerNode(std::string("$stop"), compiler_nodes, nullptr);
}
