#include "Parser.h"
 
Parser::Parser()
{
}

Parser::~Parser()
{
}

void Parser::ParseFunction()
{
	Token currentToken = Compiler::GetNext();

	if (IsNextTokenReturnType())
	{
		Token returnType = GetNext();
		Token functionName = GetNext();

		Match(TokenType::OpenBracket);

		// Set the parameters
		SymbolTable symbolTable;
		std::vector<Symbol> parameters;
		while (PeekNext()->Type != TokenType::CloseBracket)
		{
			if (PeekNext()->Type == TokenType::Seperator)
			{
				GetNext(); // remove seperator so you can add a new parameter
			}
			else
			{
				Token parameter = GetNext();
				Symbol parameterSymbol = Symbol(parameter.Value, parameter.Type, SymbolKind::Parameter);

				if (!symbolTable.HasSymbol(parameterSymbol.name) &&
					std::find(parameters.begin(), parameters.end(), parameter.Value) == parameters.end())
				{
					symbolTable.AddSymbol(parameterSymbol);
				}
				else
					std::runtime_error("Parameter name is already in use");
			}
		}

		Match(TokenType::OpenCurlyBracket);

		// TODO: Set all the nodes of this function

		Match(TokenType::CloseCurlyBracket);

		// Create a subrouyine and add it to the subroutine rable
		Subroutine routine = Subroutine(functionName.Value, returnType.Type, SubroutineKind::Function, symbolTable);
		subroutineTable.AddSubroutine(routine);
	}
	else
		std::runtime_error("Expected return type");
}

// Also check and parse if-else statement
CompilerNode Parser::ParseIfStatement()
{
	Token currentToken = Compiler::GetNext();
	bool hasPartner = false;

	int compilerNodesPos = compilerNodes->size();

	std::list<CompilerNode> innerIfStatementNodes;
	std::list<CompilerNode> innerElseStatementNodes;
	CompilerNode statementNode;
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

	statementNode = ParseExpression();

	Match(TokenType::CloseBracket);
	Match(TokenType::OpenCurlyBracket);

	while ((currentToken = GetNext()).Type != TokenType::CloseCurlyBracket)
	{
		switch (currentToken.Type)
		{
		case TokenType::If:
			innerIfStatementNodes.push_back(ParseIfStatement());
			break;
		case TokenType::While:
			ParseLoopStatement();
			break;
		case TokenType::Identifier:
			innerIfStatementNodes.push_back(ParseAssignmentStatement());
			break;
		default:
			throw std::runtime_error("No statement found");
			break;
		}
	}

	if (hasPartner)
	{
		Match(TokenType::Else);
		Match(TokenType::OpenCurlyBracket);

		while ((currentToken = GetNext()).Type != TokenType::CloseCurlyBracket)
		{
			switch (currentToken.Type)
			{
			case TokenType::If:
				innerElseStatementNodes.push_back(ParseIfStatement());
				break;
			case TokenType::While:
				ParseLoopStatement();
				break;
			case TokenType::Identifier:
				innerElseStatementNodes.push_back(ParseAssignmentStatement());
				break;
			default:
				throw std::runtime_error("No statement found");
				break;
			}
		}
	}

	std::vector<std::string> doNothing;
	CompilerNode jumpTo = CompilerNode("$doNothing", "");

	statementNode.SetJumpTo(jumpTo);

	std::list<CompilerNode>::iterator it;
	it = compilerNodes->begin();
	//make it point to the correct compilernode
	for (int i = 0; i < compilerNodesPos; i++)
	{
		it++;
	}
	compilerNodes->insert(it, statementNode);

	std::list<CompilerNode>::const_iterator iterator;
	for (iterator = innerIfStatementNodes.begin(); iterator != innerIfStatementNodes.end(); ++iterator) {
		compilerNodes->insert(it, *iterator);
	}

	for (iterator = innerElseStatementNodes.begin(); iterator != innerElseStatementNodes.end(); ++iterator) {
		compilerNodes->insert(it, *iterator);
	}

	compilerNodes->push_back(jumpTo);

	return endNode;
}

CompilerNode Parser::ParseExpression()
{
	CompilerNode parsedExpr = ParseRelationalExpression();
	while (IsNextTokenLogicalOp())
	{
		Token logicalOp = GetNext();
		CompilerNode secondParsedExpr = ParseRelationalExpression();
		std::vector<CompilerNode> parameters;

		switch (logicalOp.Type)
		{
		case TokenType::And:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = CompilerNode("$and", parameters, nullptr);
			break;
		case TokenType::Or:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = CompilerNode("$or", parameters, nullptr);
			break;
		}
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

		switch (relOp.Type)
		{
		case TokenType::LowerThan:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = CompilerNode("$less", parameters, nullptr);
			break;
		case TokenType::LowerOrEqThan:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = CompilerNode("$lessOrEq", parameters, nullptr);
			break;
		case TokenType::GreaterThan:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = CompilerNode("$greater", parameters, nullptr);
			break;
		case TokenType::GreaterOrEqThan:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = CompilerNode("$greaterOrEq", parameters, nullptr);
			break;
		case TokenType::Comparator:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = CompilerNode("$equals", parameters, nullptr);
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
		return CompilerNode("$value", token.Value);
	}
	else if (token.Type == TokenType::Identifier)
	{
		std::string identifier = token.Value;
		
		Symbol* symbol = symbolTable.GetSymbol(identifier);
		if (symbol == nullptr)
			throw SymbolNotFoundException("");

		return CompilerNode("$getVariable", symbol->name);
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
	std::vector<TokenType> operators{ TokenType::And, TokenType::Or };
	return std::find(operators.begin(), operators.end(), PeekNext()) != operators.end();
}

bool Parser::IsNextTokenRelationalOp()
{
	std::vector<TokenType> operators{ TokenType::GreaterThan, TokenType::GreaterOrEqThan, TokenType::LowerThan, TokenType::LowerOrEqThan };
	return std::find(operators.begin(), operators.end(), PeekNext()) != operators.end();
}

bool Parser::IsNextTokenAddOp()
{
	std::vector<TokenType> operators{ TokenType::OperatorPlus, TokenType::OperatorMinus };
	return std::find(operators.begin(), operators.end(), PeekNext()) != operators.end();
}

bool Parser::IsNextTokenMulOp()
{
	std::vector<TokenType> operators{ TokenType::OperatorMultiply, TokenType::OperatorDivide, TokenType::OperatorRaised };
	return std::find(operators.begin(), operators.end(), PeekNext()) != operators.end();
}

bool Parser::IsNextTokenUniOp()
{
	std::vector<TokenType> operators{ TokenType::UniOperatorPlus, TokenType::UniOperatorMinus };
	return std::find(operators.begin(), operators.end(), PeekNext()) != operators.end();
}

bool Parser::IsNextTokenReturnType()
{
	std::vector<TokenType> operators{ TokenType::Void, TokenType::None, TokenType::Float };
	return std::find(operators.begin(), operators.end(), PeekNext()) != operators.end();
}

/*
Parse while and for loops
@param standard compilerNodes.size()
*/
void Parser::ParseLoopStatement()
{
	Token currentToken = Compiler::GetNext();
	bool forLoop = false;
	
	int compilerNodesPos = compilerNodes->size();

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
	CompilerNode jumpTo = CompilerNode("$doNothing", "");

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
	std::vector<CompilerNode> nodeParameters;
	CompilerNode *arithmeticalNode = nullptr;
	CompilerNode endNode;

	Token currentToken = GetNext();
	if (currentToken.Type == TokenType::Identifier)
	{
		nodeParameters.push_back(CompilerNode("$identifier", currentToken.Value));
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
		nodeParameters.push_back(CompilerNode("$value", currentToken.Value));
	}
	else
	{
		arithmeticalNode = &ParseExpression();
	}

	if (arithmeticalNode != nullptr)
	{
		nodeParameters.push_back(*arithmeticalNode);
	}
	
	endNode = CompilerNode(expression, nodeParameters, nullptr);

	return endNode;
}
