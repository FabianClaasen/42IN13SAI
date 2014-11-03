#include "InternalFunction.h"

InternalFunction::InternalFunction(Compiler* compiler) : compiler(compiler)
{
}

InternalFunction::~InternalFunction()
{
	delete(compiler);
}

CompilerNode InternalFunction::GetInternalFunction(TokenType type)
{
	switch (type)
	{
		// Default functions
	case TokenType::Stop:
		return getCompilerNode("$stop", 0);
	case  TokenType::PrintLine:
		return getCompilerNode("$prnt", 1);

		// Math functions
	case TokenType::Cosine:
		return getCompilerNode("$cos", 1);
	case TokenType::Sine:
		return getCompilerNode("$sin", 1);
	case TokenType::Tangent:
		return getCompilerNode("$tan", 1);
	case TokenType::Square:
		return getCompilerNode("$sqr", 1);
	case TokenType::Power:
		return getCompilerNode("$pow", 2);
	case TokenType::SquareRoot:
		return getCompilerNode("$sqrt", 1);
	case TokenType::CubeRoot:
		return getCompilerNode("$cbrt", 1);
	case TokenType::Degree:
		return getCompilerNode("$deg", 1);
	case TokenType::Radiant:
		return getCompilerNode("$rad", 1);
	case TokenType::Percent:
		return getCompilerNode("$perc", 2);
	case  TokenType::PerMillage:
		return getCompilerNode("$prom", 2);
	case TokenType::NormalLog:
		return getCompilerNode("$log", 2); // or 1
	case TokenType::Nlog:
		return getCompilerNode("$ln", 1);
	case TokenType::Modulo:
		return getCompilerNode("$mod", 2);
	default:
		break;
	}
	return CompilerNode();
}

std::vector<CompilerNode*> InternalFunction::parseParameters(int expectedParams)
{
	// Check and parse all the parameters
	std::vector<CompilerNode*> compiler_nodes;
	//Match(TokenType::OpenBracket);
	while (expectedParams > 0) {
		compiler_nodes.push_back(Parser(compiler).ParseExpression());
		expectedParams--;
	}
	//if (--expectedParams > 0)
	//Match(TokenType::Seperator);
	//}
	//Match(TokenType::CloseBracket);

	return compiler_nodes;
}

CompilerNode InternalFunction::getCompilerNode(std::string functionName, int params) {
	std::vector<CompilerNode*> compiler_nodes = parseParameters(params);
	return CompilerNode(functionName, compiler_nodes, nullptr, false);
}