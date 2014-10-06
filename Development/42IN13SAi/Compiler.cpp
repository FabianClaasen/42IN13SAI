#include "Compiler.h"
#include "CompilerNode.h"
#include "Token.h"

#include <iterator>
#include <string>
#include <list>

Token Compiler::peekNext()
{
	std::list<Token>::iterator it = tokenizerTokens.begin();
	std::advance(it, currentToken + 1);

	return *it;
}

Token Compiler::getNext()
{
	std::list<Token>::iterator it = tokenizerTokens.begin();
	std::advance(it, ++currentToken);

	return *it;
}

void Compiler::parseIfStatement()
{

}

void Compiler::parseWhileStatement()
{

}

void Compiler::parseAssignmentStatement()
{

}

void Compiler::parseStatement()
{
	switch (peekNext().Type)
	{
	case TokenType::If:
		parseIfStatement();
		break;
	case TokenType::While:
		parseWhileStatement();
		break;
	case TokenType::Identifier:
		parseAssignmentStatement();
		break;
	default:
		throw new exception("Statement expected");
		break;
	}
}

void Compiler::compile()
{
	while (peekNext != nullptr)
	{
		parseStatement();
	}
}

Compiler::Compiler(std::list<Token> tokens)
{
	tokenizerTokens = tokens;

	compile();
}


Compiler::~Compiler()
{
}
