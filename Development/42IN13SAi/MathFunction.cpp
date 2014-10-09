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

CompilerNode MathFunction::CosFunction(Token identifier) {
	return CompilerNode(std::string("$cos"),parseParameters(),nullptr);
}

CompilerNode MathFunction::SinFunction(Token identifier) {
	return CompilerNode(std::string("$sin"), parseParameters(), nullptr);
}

CompilerNode MathFunction::TanFunction(Token identifier) {
	return CompilerNode(std::string("$tan"), parseParameters(), nullptr);
}

std::vector<CompilerNode> MathFunction::parseParameters()
{
	// Create list for compilernode
	std::vector<CompilerNode> compiler_nodes;

	Match(TokenType::OpenBracket);
	// Parse expression to Parser class
	CompilerNode node = parser->ParseExpression();
	Match(TokenType::CloseBracket);

	compiler_nodes.push_back(node);
}