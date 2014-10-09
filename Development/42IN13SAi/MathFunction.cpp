#include "MathFunction.h"
#include "TokenType.h"
#include "Token.h"
#include "Parser.h"

MathFunction::MathFunction()
{
}


MathFunction::~MathFunction()
{

}

CompilerNode MathFunction::CosFunction() {
	std::vector<CompilerNode> compiler_nodes = parseParameters();
	CheckNumberOfParameters(compiler_nodes, 1);
	return CompilerNode(std::string("$cos"), compiler_nodes, nullptr);
}

CompilerNode MathFunction::SinFunction() {
	std::vector<CompilerNode> compiler_nodes = parseParameters();
	return CompilerNode(std::string("$sin"), compiler_nodes, nullptr);
}

CompilerNode MathFunction::TanFunction() {
	std::vector<CompilerNode> compiler_nodes = parseParameters();
	return CompilerNode(std::string("$tan"), compiler_nodes, nullptr);
}

