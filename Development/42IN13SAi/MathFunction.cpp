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
	return CompilerNode(std::string("$cos"),parseParameters(),nullptr);
}

CompilerNode MathFunction::SinFunction() {
	return CompilerNode(std::string("$sin"), parseParameters(), nullptr);
}

CompilerNode MathFunction::TanFunction() {
	return CompilerNode(std::string("$tan"), parseParameters(), nullptr);
}

