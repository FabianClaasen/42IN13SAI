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
		std::vector<CompilerNode> parameters;

		/*switch (relOp.Type)
		{
		case TokenType::LowerThan:
			std::vector<CompilerNode> parameters;
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
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
		}*/
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
		std::vector<CompilerNode> parameters;

		switch (addOp.Type)
		{
		case TokenType::OperatorPlus:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = CompilerNode("$add", parameters, nullptr);
			break;
		case TokenType::OperatorMinus:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
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
		std::vector<CompilerNode> parameters;

		switch (mullOp.Type)
		{
		case TokenType::OperatorMultiply:
			parameters.push_back(term);
			parameters.push_back(secondTerm);
			term = CompilerNode("$mul", parameters, nullptr);
			break;
		case TokenType::OperatorDivide:
			parameters.push_back(term);
			parameters.push_back(secondTerm);
			term = CompilerNode("$div", parameters, nullptr);
			break;
		case TokenType::OperatorRaised:
			parameters.push_back(term);
			parameters.push_back(secondTerm);
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
		std::vector<CompilerNode> parameters;

		switch (uniOp.Type)
		{
		case TokenType::UniOperatorPlus:
			parameters.push_back(term);
			term = CompilerNode("$uniPlus", parameters, nullptr);
			Match(TokenType::EOL);
			break;
			parameters.push_back(term);
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
		std::vector<std::string> parameters;
		parameters.push_back(token.Value);
		return CompilerNode("$value", parameters, nullptr);
	}
	else if (token.Type == TokenType::Identifier)
	{
		std::string identifier = token.Value;
		
		Symbol* symbol = symbolTable.GetSymbol(identifier);
		if (symbol == nullptr)
			throw SymbolNotFoundException("");

		std::vector<std::string> parameters;
		parameters.push_back(symbol->name);
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
@param standard compilerNodes.size()
*/
void Parser::ParseLoopStatement(int compilerNodesPos)
{
	Token currentToken = Compiler::GetNext();
	bool forLoop = false;
	/*int compilerNodesPos = compilerNodes->size();*/

	std::vector<CompilerNode> nodeParameters;
	std::string statementExpression;

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
			ParseLoopStatement(compilerNodesPos + 1 + innerStatementNodes.size());
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
	CompilerNode jumpTo = CompilerNode("$doNothing", doNothing, nullptr);

	statementNode = CompilerNode(statementExpression, nodeParameters, &jumpTo);

	std::list<CompilerNode>::iterator it;
	it = compilerNodes->begin();
	//make it point to the correct compilernode
	for (int i = 0; i < compilerNodesPos; i++)
	{
		it++;
	}
	compilerNodes->insert(it, statementNode);

	std::list<CompilerNode>::const_iterator iterator;
	for (iterator = innerStatementNodes.begin(); iterator != innerStatementNodes.end(); ++iterator) {
		compilerNodes->insert(it, *iterator);
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
	CompilerNode *arithmeticalNode = nullptr;
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
		arithmeticalNode = &node;
	}

	if (arithmeticalNode != nullptr)
	{
		std::vector<CompilerNode> nodeParameters;

		nodeParameters.push_back(CompilerNode("$identifier", stringParameters, nullptr));
		nodeParameters.push_back(*valueNode);
		nodeParameters.push_back(*arithmeticalNode);

		endNode = CompilerNode(expression, nodeParameters, nullptr);
	}
	else
	{
		endNode = CompilerNode(expression, stringParameters, nullptr);
	}

	return endNode;
}
