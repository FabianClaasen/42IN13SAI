#include "Compiler.h"
#include "CompilerNode.h"
#include "Token.h"

#include <iterator>
#include <string>
#include <list>

/*
	Check what the next token is
*/
Token Compiler::peekNext()
{
	std::list<Token>::iterator it = tokenizerTokens.begin();
	std::advance(it, currentToken + 1);

	return *it;
}

/*
	Get the next token
*/
Token Compiler::getNext()
{
	std::list<Token>::iterator it = tokenizerTokens.begin();

	if (&Compiler::peekNext() != nullptr)
	{
		std::advance(it, ++currentToken);
	}
	else
	{
		throw new exception("Token missing");
	}

	return *it;
}

/*
	Also check and parse if-else statement
*/
void Compiler::parseIfStatement()
{

}

/*
	Parse while and for loops
*/
void Compiler::parseLoopStatement()
{

}

/*
	Return the type of the value from a token
*/
string Compiler::getTokenValueType(Token currentToken)
{
	if (currentToken.Type == TokenType::Boolean)
	{
		return "Boolean";
	}
	else if (currentToken.Type == TokenType::Integer)
	{
		return "Integer";
	}
	else if (currentToken.Type == TokenType::Double)
	{
		return "Double";
	}
	else
	{
		return "String";
	}
}

/*
	Also parse (standard) Arithmetical operations
*/
void Compiler::parseAssignmentStatement()
{
	std::string expression;
	std::string identifier;
	std::string value;

	Token currentToken = getNext();
	if (currentToken.Type == TokenType::Identifier)
	{
		identifier = currentToken.Value;
	}
	else
	{
		throw new exception("Identifier expected");
	}
	
	currentToken = getNext();

	if (currentToken.Type == TokenType::Equals)
	{
		expression = "$assignment";
	}

	currentToken = getNext();

	value = currentToken.Value;
	expression = expression + getTokenValueType(currentToken);
}

/*
	Check what to parse
*/
void Compiler::parseStatement()
{
	switch (peekNext().Type)
	{
	case TokenType::If:
		parseIfStatement();
		break;
	case TokenType::While:
		parseLoopStatement();
		break;
	case TokenType::Identifier:
		parseAssignmentStatement();
		break;
	default:
		throw new exception("Statement expected");
		break;
	}
}

/*
	keep parsing as long as there are tokens 
*/
void Compiler::compile()
{
	while (&Compiler::peekNext() != nullptr)
	{
		parseStatement();
	}
}

/*
	Constructor
*/
Compiler::Compiler(std::list<Token> tokens)
{
	tokenizerTokens = tokens;

	compile();
}

/*
	Destructor
*/
Compiler::~Compiler()
{
}
