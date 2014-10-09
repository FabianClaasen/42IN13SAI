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

    Compiler::Match(TokenType::OpenBracket);

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
    return parsedExpr;
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

	if (token.Type == TokenType::Float)
	{

	}
	else if (token.Type == TokenType::Identifier)
	{
		std::string identifier = token.Value;
		
		/*if (variable == null)
			throw std::exception();*/
	}
	else if (token.Type == TokenType::OpenBracket)
	{
		CompilerNode expr = ParseExpression();
		Match(TokenType::CloseBracket);
		return expr;
	}

	return CompilerNode();
}

/*
Parse while and for loops
*/
void Parser::ParseLoopStatement()
{
	Token currentToken = Compiler::GetNext();
	bool forLoop = false;

	std::vector<CompilerNode> nodeParameters;
	std::string statementExpression;
	std::string innerStatementExpression;

	CompilerNode statementNode;

	std::list<CompilerNode> innerStatementNodes;

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
		nodeParameters.push_back(ParseAssignmentStatement());
		nodeParameters.push_back(ParseExpression());
		nodeParameters.push_back(ParseAddExpression());

		statementExpression = "$forLoop";
	}
	else
	{
		nodeParameters.push_back(ParseExpression());

		statementExpression = "$whileLoop";
	}

	Match(TokenType::CloseBracket);
	Match(TokenType::OpenCurlyBracket);

	while ((currentToken = GetNext()).Type != TokenType::CloseCurlyBracket)
	{
		switch (currentToken.Type)
		{
		case TokenType::If:
			innerStatementNodes.push_back(ParseIfStatement());
			break;
		case TokenType::While:
			ParseLoopStatement();
			break;
		case TokenType::Identifier:
			innerStatementNodes.push_back(ParseAssignmentStatement());
			break;
		default:
			throw std::runtime_error("No statement found");
			break;
		}
	}

	std::vector<std::string> doNothing;
	CompilerNode jumpTo = CompilerNode("$doNothing", &doNothing, nullptr);

	statementNode = CompilerNode(statementExpression, &nodeParameters, &jumpTo);

	compilerNodes->push_back(statementNode);

	std::list<CompilerNode>::const_iterator iterator;
	for (iterator = innerStatementNodes.begin(); iterator != innerStatementNodes.end(); ++iterator) {
		compilerNodes->push_back(*iterator);
	}

	compilerNodes->push_back(jumpTo);
}

/*
Also parse (standard) Arithmetical operations
*/
CompilerNode Parser::ParseAssignmentStatement()
{
	std::string expression = "";
	std::vector<std::string> stringParameters;
	CompilerNode *valueNode = nullptr;
	CompilerNode endNode;

	Token currentToken = GetNext();
	if (currentToken.Type == TokenType::Identifier)
	{
		stringParameters.push_back(currentToken.Value);
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

	if (PeekNext()->Type == TokenType::EOL)
	{
		stringParameters.push_back(currentToken.Value);
		expression = expression;
	}
	else
	{
		CompilerNode node = ParseExpression();
		valueNode = &node;
	}

	if (valueNode != nullptr)
	{
		std::vector<CompilerNode> nodeParameters;

		nodeParameters.push_back(CompilerNode("$identifier", &stringParameters, nullptr));
		nodeParameters.push_back(*valueNode);

		endNode = CompilerNode(expression, &nodeParameters, nullptr);
	}
	else
	{
		endNode = CompilerNode(expression, &stringParameters, nullptr);
	}

	return endNode;
}
