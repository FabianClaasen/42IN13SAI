#include "InternalFunction.h"

#include "MathFunction.h"
#include "DefaultFunction.h"
#include "PhysicsFunction.h"
#include "Parser.h"

InternalFunction::InternalFunction()
{
	
}

InternalFunction::~InternalFunction()
{
	
}

CompilerNode InternalFunction::GetInternalFunction(Token identifier)
{
	switch (identifier.Type)
	{
	case TokenType::Cosine:
		return mathFunction->CosFunction();
	case TokenType::Sine:
		return mathFunction->SinFunction();
	default:
		break;
	}
    return CompilerNode();
}

std::vector<CompilerNode> InternalFunction::parseParameters(int expectedParams)
{
	// Check and parse all the parameters
	std::vector<CompilerNode> compiler_nodes;
	Match(TokenType::OpenBracket);
	while (expectedParams > 0) {
		CompilerNode node = parser->ParseExpression();
		compiler_nodes.push_back(node);
		if (--expectedParams > 0)
			Match(TokenType::Seperator);
		else
			Match(TokenType::CloseBracket);
	}
	return compiler_nodes;
}

void InternalFunction::CheckNumberOfParameters(std::vector<CompilerNode> compiler_nodes, int number)
{
	if (!compiler_nodes.size() == number)
	{
		throw std::runtime_error("InvalidNumberOfParameterException");
	}
}
