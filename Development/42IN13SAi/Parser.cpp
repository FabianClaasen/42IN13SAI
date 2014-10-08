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
CompilerNode Parser::ParseIfStatement()
{
	Token currentToken = Compiler::GetNext();
	bool hasPartner = false;

	CompilerNode endNode;

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

	return endNode;
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
CompilerNode Parser::ParseLoopStatement()
{
	Token currentToken = Compiler::GetNext();
	bool forLoop = false;

	CompilerNode endNode;

	if (currentToken.Type != TokenType::While || currentToken.Type != TokenType::ForLoop)
	{
		throw std::runtime_error("Expected a loop keyword");
	}
	else
	{
		if (currentToken.Type == TokenType::ForLoop)
		{
			forLoop = true;
		}
	}

	Match(TokenType::OpenBracket);

	if (forLoop)
	{
		CompilerNode assignmentNode = ParseAssignmentStatement();
		CompilerNode expressionNode = ParseExpression();
		CompilerNode addExpressionNode = ParseAddExpression();
	}
	else
	{
		CompilerNode expressionNode = ParseExpression();
	}

	Match(TokenType::CloseBracket);
	Match(TokenType::OpenCurlyBracket);

	while ((currentToken = GetNext()).Type != TokenType::CloseCurlyBracket)
	{
		//TODO parse stuff in the loop
	}

	return endNode;
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
CompilerNode Parser::ParseAssignmentStatement()
{
	std::string expression = "";
	std::string identifier = "";
	std::string valueString = "";
	CompilerNode *valueNode = nullptr;
	CompilerNode endNode;

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

	if (parser.PeekNext().Type == TokenType::EOL)
	{
		valueString = currentToken.Value;
		expression = expression;
	}
	else
	{
		CompilerNode node = ParseExpression();
		valueNode = &node;
	}

	if (valueString == "" && valueNode != nullptr)
	{
		endNode = CompilerNode(expression, identifier, *valueNode);
	}
	else
	{
		endNode = CompilerNode(expression, identifier, valueString);
	}

	return endNode;
}
