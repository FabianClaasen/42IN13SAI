#include "Parser.h"

Parser::Parser()
{
}

Parser::~Parser()
{
}

/*
Also check and parse if-else statement
*/
void Parser::ParseIfStatement()
{
	Token currentToken = Compiler::GetNext();
	bool hasPartner = false;

	if (currentToken.Type == TokenType::If)
	{
		if (currentToken.Partner != nullptr)
		{
			hasPartner = true;
		}
	}
	else
	{
		throw std::runtime_error("Expected if keyword");
	}

	Match(TokenType::OpenBracket);

	ParseExpression();

	Match(TokenType::CloseBracket);
	Match(TokenType::OpenCurlyBracket);

	while ((currentToken = GetNext()).Type != TokenType::CloseCurlyBracket)
	{
		//TODO parse stuff in if statement
	}

	if (hasPartner)
	{
		//TODO parse else statement
	}
}

CompilerNode Parser::ParseExpression()
{
	CompilerNode parsedExpr = ParseRelationalExpression();
}

CompilerNode Parser::ParseRelationalExpression()
{
	CompilerNode parsedExpr = ParseAddExpression();
	return parsedExpr;
}

CompilerNode Parser::ParseAddExpression()
{
	CompilerNode parsedExpr = ParseMulExpression();
	return parsedExpr;
}

CompilerNode Parser::ParseMulExpression()
{
	CompilerNode parsedExpr = ParseUniExpression();
	return parsedExpr;
}

CompilerNode Parser::ParseUniExpression()
{
	CompilerNode term = ParseTerm();
	return term;
}

CompilerNode Parser::ParseTerm()
{
	Token token = GetNext();

	if (token.Type == TokenType::Integer)
	{

	}
	else if (token.Type == TokenType::Identifier)
	{
		string identifier = token.Value;
		
		/*if (variable == null)
			throw std::exception();*/
	}
	else if (token.Type == TokenType::OpenBracket)
	{
		CompilerNode expr = ParseExpression();
		Match(TokenType::CloseBracket);
		return expr;
	}

	return;
}

/*
Parse while and for loops
*/
void Parser::ParseLoopStatement()
{

}

/*
Return the type of the value from a token
*/
string Parser::GetTokenValueType(Token currentToken)
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
void Parser::ParseAssignmentStatement()
{
	std::string expression;
	std::string identifier;
	std::string value;

	Token currentToken = GetNext();
	if (currentToken.Type == TokenType::Identifier)
	{
		identifier = currentToken.Value;
	}
	else
	{
		throw std::runtime_error("Identifier expected");
	}

	currentToken = GetNext();

	if (currentToken.Type == TokenType::Equals)
	{
		expression = "$assignment";
	}

	currentToken = GetNext();

	//if (parser.PeekNext().Type == TokenType::)

	value = currentToken.Value;
	expression = expression + GetTokenValueType(currentToken);
}
