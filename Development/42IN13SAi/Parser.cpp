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
	while (IsNextTokenUniOp())
	{

	}

    return parsedExpr;
}

CompilerNode Parser::ParseRelationalExpression()
{
	CompilerNode parsedExpr = ParseAddExpression();
	while (IsNextTokenRelationalOp())
	{
		Token relOp = GetNext();
		CompilerNode secondParsedExpr = ParseAddExpression();

		switch (relOp.Type)
		{
		case TokenType::LowerThan:
			std::vector<CompilerNode>* parameters = new std::vector<CompilerNode>();
			parameters->push_back(parsedExpr);
			parameters->push_back(secondParsedExpr);
			parsedExpr = CompilerNode("$add", parameters, nullptr);
			break;
		case TokenType::LowerOrEqThan:
			break;
		case TokenType::GreaterThan:
			break;
		case TokenType::GreaterOrEqThan:
			break;
		case TokenType::Comparator:
			break;
		}
	}

	return parsedExpr;
}

CompilerNode Parser::ParseAddExpression()
{
	CompilerNode parsedExpr = ParseMulExpression();
	while (IsNextTokenAddOp())
	{
		Token addOp = GetNext();
		CompilerNode secondParsedExpr = ParseMulExpression();

		switch (addOp.Type)
		{
		case TokenType::OperatorPlus:
			std::vector<CompilerNode>* parameters = new std::vector<CompilerNode>();
			parameters->push_back(parsedExpr);
			parameters->push_back(secondParsedExpr);
			parsedExpr = CompilerNode("$add", parameters, nullptr);
			break;
		case TokenType::OperatorMinus:
			std::vector<CompilerNode>* parameters = new std::vector<CompilerNode>();
			parameters->push_back(parsedExpr);
			parameters->push_back(secondParsedExpr);
			parsedExpr = CompilerNode("$min", parameters, nullptr);
			break;
		}
	}

	return parsedExpr;
}

CompilerNode Parser::ParseMulExpression()
{
	CompilerNode term = ParseUniExpression();
	while (IsNextTokenMulOp())
	{
		Token mullOp = GetNext();
		CompilerNode secondTerm = ParseUniExpression();

		switch (mullOp.Type)
		{
		case TokenType::OperatorMultiply:
			std::vector<CompilerNode>* parameters = new std::vector<CompilerNode>();
			parameters->push_back(term);
			parameters->push_back(secondTerm);
			term = CompilerNode("$mul", parameters, nullptr);
			break;
		case TokenType::OperatorDivide:
			std::vector<CompilerNode>* parameters = new std::vector<CompilerNode>();
			parameters->push_back(term);
			parameters->push_back(secondTerm);
			term = CompilerNode("$div", parameters, nullptr);
			break;
		case TokenType::OperatorRaised:
			std::vector<CompilerNode>* parameters = new std::vector<CompilerNode>();
			parameters->push_back(term);
			parameters->push_back(secondTerm);
			term = CompilerNode("$raise", parameters, nullptr);
			break;
		}
	}

	return term;
}

CompilerNode Parser::ParseUniExpression()
{
	CompilerNode term = ParseTerm();
	while (IsNextTokenUniOp())
	{
		Token uniOp = GetNext();

		switch (uniOp.Type)
		{
		case TokenType::UniOperatorPlus:
			std::vector<CompilerNode>* parameters = new std::vector<CompilerNode>();
			parameters->push_back(term);
			term = CompilerNode("$uniPlus", parameters, nullptr);
			Match(TokenType::EOL);
			break;
		case TokenType::UniOperatorMinus:
			std::vector<CompilerNode>* parameters = new std::vector<CompilerNode>();
			parameters->push_back(term);
			term = CompilerNode("$uniMin", parameters, nullptr);
			Match(TokenType::EOL);
			break;
		}
	}

	return term;
}

CompilerNode Parser::ParseTerm()
{
	Token token = GetNext();

	if (token.Type == TokenType::Float)
	{
		std::vector<std::string>* parameters = new std::vector<std::string>();
		parameters->push_back(token.Value);
		return CompilerNode("$value", parameters, nullptr);
	}
	else if (token.Type == TokenType::Identifier)
	{
		std::string identifier = token.Value;
		
		Symbol* symbol = symbolTable.GetSymbol(identifier);
		if (symbol == nullptr)
			throw SymbolNotFoundException("");

		std::vector<std::string>* parameters = new std::vector<std::string>();
		parameters->push_back(symbol->name);
		return CompilerNode("$getVariable", parameters, nullptr);
	}
	else if (token.Type == TokenType::OpenBracket)
	{
		CompilerNode expr = ParseExpression();
		Match(TokenType::CloseBracket);
		return expr;
	}

	return CompilerNode();
}

bool Parser::IsNextTokenLogicalOp()
{
	return false;
}

bool Parser::IsNextTokenRelationalOp()
{
	return false;
}

bool Parser::IsNextTokenAddOp()
{
	return false;
}

bool Parser::IsNextTokenMulOp()
{
	return false;
}

bool Parser::IsNextTokenUniOp()
{
	return false;
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
