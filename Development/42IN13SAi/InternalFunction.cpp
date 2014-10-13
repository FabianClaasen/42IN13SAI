#include "InternalFunction.h"
#include "MathFunction.h"
#include "DefaultFunction.h"
#include "PhysicsFunction.h"
#include "Parser.h"

InternalFunction::InternalFunction()
{
	// Do something?
}

InternalFunction::~InternalFunction()
{
	
}

CompilerNode InternalFunction::GetInternalFunction(Token identifier)
{
	switch (identifier.Type)
	{
		// Default functions
	case TokenType::Stop:
		return getFunctionNode("$stop",1);
	case  TokenType::PrintLine:
		return getFunctionNode("$prnt",1);
		// Math functions
	case TokenType::Cosine:
		return getFunctionNode("$cos",1);
	case TokenType::Sine:
		return getFunctionNode("$sin", 1);
	case TokenType::Tangent:
		return getFunctionNode("$tan", 1);
	case TokenType::Square:
		return getFunctionNode("$sqr", 1);
	case TokenType::Power:
		return getFunctionNode("$pow", 2);
	case TokenType::SquareRoot:
		return getFunctionNode("$sqrt", 1);
	case TokenType::CubeRoot:
		return getFunctionNode("$cbrt", 1);
	case TokenType::Degree:
		return getFunctionNode("$deg", 1);
	case TokenType::Radiant:
		return getFunctionNode("$rad", 1);
	case TokenType::Percent:
		return getFunctionNode("$perc", 2);
	case  TokenType::PerMillage:
		return getFunctionNode("$prom", 2);
	case TokenType::NormalLog:
		return getFunctionNode("$log", 2); // or 1
	case TokenType::Nlog:
		return getFunctionNode("$ln", 1);
	case TokenType::Modulo:
		return getFunctionNode("$mod", 2);
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
	}
	Match(TokenType::CloseBracket);

	return compiler_nodes;
}

CompilerNode InternalFunction::getFunctionNode(std::string functionName, int params) {
	std::vector<CompilerNode> compiler_nodes = parseParameters(params);
	return CompilerNode(functionName, compiler_nodes, nullptr);
}


