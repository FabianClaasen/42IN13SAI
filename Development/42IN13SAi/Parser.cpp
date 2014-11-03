#include "Parser.h"

Parser::Parser(Compiler* compiler) : compiler(compiler)
{
}

Parser::Parser(Compiler* compiler, std::vector<Token> tokens) : compiler(compiler)
{
	compiler->SetTokenList(tokens);
}

Parser::~Parser()
{
	//delete(compiler);
}

#pragma region ParseStatementMethods
void Parser::ParseFunction()
{
	Token currentToken = compiler->GetNext();

	SymbolTable symbolTable;

    if (currentToken.Type == TokenType::MainFunction || IsNextTokenReturnType())
	{
        Token returnType;
        Token functionName;
        if (currentToken.Type == TokenType::MainFunction)
        {
            returnType = Token(0,0,0, "", TokenType::Void, nullptr);
            functionName = Token(0,0,0, "main", TokenType::MainFunction, nullptr);
        }
        else
        {
            returnType = compiler->GetNext();
            functionName = compiler->GetNext();
        }

		compiler->Match(TokenType::OpenBracket);

		// Set the parameters
		while (compiler->PeekNext()->Type != TokenType::CloseBracket)
		{
			if (compiler->PeekNext()->Type == TokenType::Seperator)
			{
				compiler->GetNext(); // remove seperator so you can add a new parameter
			}
			else
			{
				Token parameter = compiler->GetNext();
				Symbol parameterSymbol = Symbol(parameter.Value, parameter.Type, SymbolKind::Parameter);
				

				if (!symbolTable.HasSymbol(parameterSymbol.name))
				{
					symbolTable.AddSymbol(parameterSymbol);
				}
				else
					throw std::runtime_error("Parameter name is already in use");
			}
		}
        
        if (compiler->PeekNext()->Type == TokenType::CloseBracket)
            compiler->GetNext();

		// Check if the functions starts and create a subroutine
		compiler->Match(TokenType::OpenMethod);
		compiler->SetSubroutine(Subroutine(functionName.Value, returnType.Type, SubroutineKind::Function, symbolTable));

		// Set all the statements inside this subroutine
		while (compiler->PeekNext()->Type != TokenType::CloseMethod && compiler->PeekNext()->Level > 1)
		{
			compiler->ParseStatement();
		}

		// Check if the subroutine is closed correctly
		// And add the subroutine to the subroutine table
		compiler->Match(TokenType::CloseMethod);
		compiler->AddSubroutine();
	}
	else
		throw std::runtime_error("Expected return type");
}

CompilerNode* Parser::ParseReturn()
{
    compiler->Match(TokenType::Return);
    
    Token returnToken = compiler->GetNext();
    
    std::vector<CompilerNode*> nodeParameters;
    nodeParameters.push_back(ParseExpression());
    
    return new CompilerNode("$ret", nodeParameters, nullptr, false);
}

//Also parse (standard) Arithmetical operations
CompilerNode Parser::ParseAssignmentStatement()
{
	std::string expression = "";
	std::vector<CompilerNode*> nodeParameters;
	CompilerNode* endNode;
	Subroutine* subroutine = compiler->GetSubroutine();

	bool newIdentifier = false;

	Token identifier;
	Token currentToken = compiler->GetNext();
	if (currentToken.Type == TokenType::Var)
	{
		newIdentifier = true;
		identifier = compiler->GetNext();
	}
	else
	{
		identifier = currentToken;
	}

	// Check if the identifier is a identifier
	if (identifier.Type != TokenType::Identifier)
		throw std::runtime_error("Identifier expected");

	// Check if the identifier exists
	if (!newIdentifier)
	{
		Symbol* symbol = GetSymbol(identifier.Value);

		if (symbol == nullptr)
			throw SymbolNotFoundException("This identifier has not been made yet");
	}
	else
	{
		Symbol* identifierSymbol;

		if (subroutine->isEmpty)
		{
			identifierSymbol = new Symbol(identifier.Value, identifier.Type, SymbolKind::Global);
		}
		else
		{
			identifierSymbol = new Symbol(identifier.Value, identifier.Type, SymbolKind::Local);
		}

		if (!compiler->HasSymbol(identifierSymbol->name))
		{
			compiler->AddSymbol(*identifierSymbol);
		}
		else
			throw std::runtime_error("Identifier name is already in use");
	}

	CompilerNode* id = new CompilerNode("$identifier", identifier.Value, nullptr);
	nodeParameters.push_back(id);

	// Check if it is an assignment or only a decleration.
	if (compiler->PeekNext()->Type == TokenType::Equals)
	{
		compiler->GetNext(); // remove the '=' token
		expression = "$assignment";
		CompilerNode* node = ParseExpression();
		nodeParameters.push_back(node);
	}

	// Check if the code is closed
	compiler->Match(TokenType::EOL);

	// Set the final node
	endNode = new CompilerNode(expression, nodeParameters, nullptr, false);

	if (compiler->GetSubroutine()->isEmpty)
		compiler->AddCompilerNode(*endNode);
	else
		compiler->GetSubroutine()->AddCompilerNode(*endNode);

	return *endNode;
}

// This function is only called when a function is called without it being in a assignment.
void Parser::ParseFunctionCall()
{
	std::vector<CompilerNode*> nodeParameters;
	Token currentToken = compiler->GetNext();

	CompilerNode* node = new CompilerNode("$functionName", currentToken.Value, false);
	nodeParameters.push_back(node);

    compiler->Match(TokenType::OpenBracket);
	while (compiler->PeekNext()->Type != TokenType::CloseBracket)
	{
		currentToken = compiler->GetNext();
		if (currentToken.Type == TokenType::Seperator)
		{
			currentToken = compiler->GetNext();
		}

		CompilerNode* node = ParseExpression();
        if (node != nullptr)
            nodeParameters.push_back(node);
	}

	compiler->Match(TokenType::CloseBracket);
	compiler->Match(TokenType::EOL);

	// Create the finall node
	CompilerNode endNode = CompilerNode("$functionCall", nodeParameters, nullptr, false);

	// Add the final node
	if (compiler->GetSubroutine()->isEmpty)
		compiler->AddCompilerNode(endNode);
	else
		compiler->GetSubroutine()->AddCompilerNode(endNode);
}

// Also check and parse if-else statement
void Parser::ParseIfStatement()
{
	Token currentToken = compiler->GetNext();
	bool hasPartner = false;

	Subroutine* subroutine = compiler->GetSubroutine();
	std::list<CompilerNode>* compilerNodes = subroutine->GetCompilerNodeCollection();
	long compilerNodesPos = compilerNodes->size();

	std::list<CompilerNode> innerIfStatementNodes;
	std::list<CompilerNode> innerElseStatementNodes;
	CompilerNode* statementNode;
	CompilerNode* endNode;

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

	compiler->Match(TokenType::OpenBracket);

	statementNode = ParseExpression();

	compiler->Match(TokenType::CloseBracket);
	compiler->Match(TokenType::OpenMethod);

	while (compiler->PeekNext()->Type != TokenType::CloseMethod)
	{
		switch (compiler->PeekNext()->Type)
		{
		case TokenType::If:
			ParseIfStatement();
			break;
		case TokenType::While:
			ParseLoopStatement();
			break;
		case TokenType::Identifier:
			ParseAssignmentStatement();
			break;
		default:
			throw std::runtime_error("No statement found");
			break;
		}
	}

	compiler->Match(TokenType::CloseMethod);

	if (hasPartner)
	{
		compiler->Match(TokenType::Else);
		compiler->Match(TokenType::OpenMethod);

		while (compiler->PeekNext()->Type != TokenType::CloseMethod)
		{
			switch (compiler->PeekNext()->Type)
			{
			case TokenType::If:
				ParseIfStatement();
				break;
			case TokenType::While:
				ParseLoopStatement();
				break;
			case TokenType::Identifier : case TokenType::Var:
				ParseAssignmentStatement();
				break;
			default:
				throw std::runtime_error("No statement found");
				break;
			}
		}

		compiler->Match(TokenType::CloseMethod);
	}

	std::vector<std::string> doNothing;
	CompilerNode jumpTo = CompilerNode("$doNothing", "", false);

	statementNode->SetJumpTo(jumpTo);

	std::list<CompilerNode>::iterator it;
	it = compilerNodes->begin();
	//make it point to the correct compilernode
	for (int i = 0; i < compilerNodesPos; i++)
	{
		it++;
	}
	compilerNodes->insert(it, *statementNode);

	std::list<CompilerNode>::const_iterator iterator;
	for (iterator = innerIfStatementNodes.begin(); iterator != innerIfStatementNodes.end(); ++iterator) {
		compilerNodes->insert(it, *iterator);
	}

	for (iterator = innerElseStatementNodes.begin(); iterator != innerElseStatementNodes.end(); ++iterator) {
		compilerNodes->insert(it, *iterator);
	}

	compilerNodes->push_back(jumpTo);
}

/*
Parse while and for loops
@param standard compilerNodes.size()
*/
void Parser::ParseLoopStatement()
{
	Token currentToken = compiler->GetNext();
	bool forLoop = false;

	Subroutine* subroutine(compiler->GetSubroutine());
	std::list<CompilerNode>* compilerNodes = subroutine->GetCompilerNodeCollection();
	long compilerNodesPos = compilerNodes->size();

	std::vector<CompilerNode*> nodeParameters;
	std::string statementExpression;

	CompilerNode statementNode;
	std::list<CompilerNode> innerStatementNodes;

	if (currentToken.Type == TokenType::While || currentToken.Type == TokenType::ForLoop)
	{
		if (currentToken.Type == TokenType::ForLoop)
		{
			forLoop = true;
		}
	}
	else
	{
		throw std::runtime_error("Expected a loop keyword");
	}

	compiler->Match(TokenType::OpenBracket);

	if (forLoop)
	{
		ParseAssignmentStatement();
		nodeParameters.push_back(ParseExpression());
		nodeParameters.push_back(ParseAddExpression());

		statementExpression = "$forLoop";
	}
	else
	{
		nodeParameters.push_back(ParseExpression());

		statementExpression = "$whileLoop";
	}

	compiler->Match(TokenType::CloseBracket);
	Token* openMethod = compiler->PeekNext();
	compiler->Match(TokenType::OpenMethod);

	while (compiler->PeekNext()->Type != TokenType::CloseMethod && compiler->PeekNext()->Level == openMethod->Level)
	{
		compiler->ParseStatement();
	}

	compiler->Match(TokenType::CloseMethod);

	// Create a do nothing, so you can jump to this when the statement is false
	std::vector<std::string> doNothing;
	CompilerNode jumpTo = CompilerNode("$doNothing", "", false);

	statementNode = CompilerNode(statementExpression, nodeParameters, &jumpTo, false);

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
#pragma endregion ParseStatementMethods

#pragma region ParseExpressionMethods
CompilerNode* Parser::ParseExpression()
{
	CompilerNode* parsedExpr = ParseRelationalExpression();
	while (IsNextTokenLogicalOp())
	{
		Token logicalOp = compiler->GetNext();
		CompilerNode* secondParsedExpr = ParseRelationalExpression();
		std::vector<CompilerNode*> parameters;

		switch (logicalOp.Type)
		{
		case TokenType::And:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = new CompilerNode("$and", parameters, nullptr, false);
			break;
		case TokenType::Or:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = new CompilerNode("$or", parameters, nullptr, false);
			break;
		}
	}

	return parsedExpr;
}

CompilerNode* Parser::ParseRelationalExpression()
{
	CompilerNode* parsedExpr = ParseAddExpression();
	while (IsNextTokenRelationalOp())
	{
		Token relOp = compiler->GetNext();
		CompilerNode* secondParsedExpr = ParseAddExpression();
		std::vector<CompilerNode*> parameters;

		switch (relOp.Type)
		{
		case TokenType::LowerThan:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = new CompilerNode("$less", parameters, nullptr, false);
			break;
		case TokenType::LowerOrEqThan:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = new CompilerNode("$lessOrEq", parameters, nullptr, false);
			break;
		case TokenType::GreaterThan:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = new CompilerNode("$greater", parameters, nullptr, false);
			break;
		case TokenType::GreaterOrEqThan:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = new CompilerNode("$greaterOrEq", parameters, nullptr, false);
			break;
		case TokenType::Comparator:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = new CompilerNode("$equals", parameters, nullptr, false);
			break;
		}
	}

	return parsedExpr;
}

CompilerNode* Parser::ParseAddExpression()
{
	CompilerNode* parsedExpr = ParseMulExpression();
	while (IsNextTokenAddOp())
	{
		Token addOp = compiler->GetNext();
		CompilerNode* secondParsedExpr = ParseMulExpression();
		std::vector<CompilerNode*> parameters;

		switch (addOp.Type)
		{
		case TokenType::OperatorPlus:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = new CompilerNode("$add", parameters, nullptr, false);
			break;
		case TokenType::OperatorMinus:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = new CompilerNode("$min", parameters, nullptr, false);
			break;
		}
	}

	return parsedExpr;
}

CompilerNode* Parser::ParseMulExpression()
{
	CompilerNode* term = ParseUniExpression();
	while (IsNextTokenMulOp())
	{
		Token mullOp = compiler->GetNext();
		CompilerNode* secondTerm = ParseUniExpression();
		std::vector<CompilerNode*> parameters;

		switch (mullOp.Type)
		{
		case TokenType::OperatorMultiply:
			parameters.push_back(term);
			parameters.push_back(secondTerm);
			term = new CompilerNode("$mul", parameters, nullptr, false);
			break;
		case TokenType::OperatorDivide:
			parameters.push_back(term);
			parameters.push_back(secondTerm);
			term = new CompilerNode("$div", parameters, nullptr, false);
			break;
		case TokenType::OperatorRaised:
			parameters.push_back(term);
			parameters.push_back(secondTerm);
			term = new CompilerNode("$raise", parameters, nullptr, false);
			break;
		}
	}

	return term;
}

CompilerNode* Parser::ParseUniExpression()
{
	CompilerNode* term = ParseTerm();

	while (IsNextTokenUniOp())
	{
		Token uniOp = compiler->GetNext();
		std::vector<CompilerNode *> parameters;

		switch (uniOp.Type)
		{
		case TokenType::UniOperatorPlus:
			parameters.push_back(term);
			term = new CompilerNode("$uniPlus", parameters, nullptr, false);
			compiler->Match(TokenType::EOL);
			break;
        case TokenType::UniOperatorMinus:
			parameters.push_back(term);
			term = new CompilerNode("$uniMin", parameters, nullptr, false);
			compiler->Match(TokenType::EOL);
			break;
		}
	}

	return term;
}

CompilerNode* Parser::ParseTerm()
{
	Token token = compiler->GetNext();

	CompilerNode* node = nullptr;

	if (token.Type == TokenType::Float)
	{
		node = new CompilerNode("$value", token.Value, false);
		return node;
	}
	else if (token.Type == TokenType::Identifier)
	{
		if (compiler->PeekNext()->Type == TokenType::OpenBracket)
		{
			return ParseFunctionCall(token);
		}
		else
		{
			std::string identifier = token.Value;

			Symbol* symbol = GetSymbol(identifier);

			if (symbol == nullptr)
				throw SymbolNotFoundException("");

			node = new CompilerNode("$getVariable", symbol->name, false);
			return node;
		}
	}
	else if (token.Type == TokenType::OpenBracket)
	{
		node = ParseExpression();
		compiler->Match(TokenType::CloseBracket);
		return node;
	}
	else if (compiler->IsInternalFunction(token.Type) && !(token.Type==TokenType::PrintLine || token.Type==TokenType::Stop))
	{
			node = InternalFunction(compiler).GetInternalFunction(token.Type);
			return node;
	}
    else if (compiler->PeekNext()->Type == TokenType::OpenBracket)
    {
        return ParseFunctionCall(token);
    }
	return node;
}

// This function is called when there is a function inside a expression
CompilerNode* Parser::ParseFunctionCall(Token token)
{
	std::vector<CompilerNode*> nodeParameters;
	CompilerNode* node = new CompilerNode("$functionName", token.Value, false);
	nodeParameters.push_back(node);
	Token currentToken = compiler->GetNext();

	while (compiler->PeekNext()->Type != TokenType::CloseBracket)
	{
		if (compiler->PeekNext()->Type == TokenType::Seperator)
		{
			currentToken = compiler->GetNext();
		}

		CompilerNode* node = ParseExpression();
		nodeParameters.push_back(node);
	}

	compiler->Match(TokenType::CloseBracket);

	CompilerNode* finalNode = new CompilerNode("$functionCall", nodeParameters, nullptr, false);
	return finalNode;
}
#pragma endregion ParseExpressionMethods

#pragma region IsNextTokenMethods
bool Parser::IsNextTokenLogicalOp()
{
	TokenType type = compiler->PeekNext()->Type;
	return type == TokenType::And ||
		type == TokenType::Or;

	/*std::vector<TokenType> operators{ TokenType::And, TokenType::Or };
	return std::find(operators.begin(), operators.end(), compiler->PeekNext()->Type) != operators.end();*/
}

bool Parser::IsNextTokenRelationalOp()
{
	TokenType type = compiler->PeekNext()->Type;
	return type == TokenType::GreaterThan ||
		type == TokenType::GreaterOrEqThan ||
		type == TokenType::LowerThan ||
		type == TokenType::LowerOrEqThan ||
		type == TokenType::Comparator;

	/*std::vector<TokenType> operators{ TokenType::GreaterThan, TokenType::GreaterOrEqThan, TokenType::LowerThan, TokenType::LowerOrEqThan };
	return std::find(operators.begin(), operators.end(), compiler->PeekNext()->Type) != operators.end();*/
}

bool Parser::IsNextTokenAddOp()
{
	TokenType type = compiler->PeekNext()->Type;
	return type == TokenType::OperatorMinus ||
		type == TokenType::OperatorPlus;

	/*std::vector<TokenType> operators{ TokenType::OperatorPlus, TokenType::OperatorMinus };
	return std::find(operators.begin(), operators.end(), compiler->PeekNext()->Type) != operators.end();*/
}

bool Parser::IsNextTokenMulOp()
{
	TokenType type = compiler->PeekNext()->Type;
	return type == TokenType::OperatorMultiply ||
		type == TokenType::OperatorDivide ||
		type == TokenType::OperatorRaised;

	/*std::vector<TokenType> operators{ TokenType::OperatorMultiply, TokenType::OperatorDivide, TokenType::OperatorRaised };
	return std::find(operators.begin(), operators.end(), compiler->PeekNext()->Type) != operators.end();*/
}

bool Parser::IsNextTokenUniOp()
{
	TokenType type = compiler->PeekNext()->Type;
	return type == TokenType::UniOperatorPlus ||
		type == TokenType::UniOperatorMinus;

	/*std::vector<TokenType> operators{ TokenType::UniOperatorPlus, TokenType::UniOperatorMinus };
	return std::find(operators.begin(), operators.end(), compiler->PeekNext()->Type) != operators.end();*/
}

bool Parser::IsNextTokenReturnType()
{
	TokenType type = compiler->PeekNext()->Type;
	return type == TokenType::Void ||
		type == TokenType::None ||
		type == TokenType::Float;

	/*std::vector<TokenType> operators{ TokenType::Void, TokenType::None, TokenType::Float };
	return std::find(operators.begin(), operators.end(), compiler->PeekNext()->Type) != operators.end();*/
}
#pragma endregion IsNextTokenMethods

// This function will return a symbol based on the identifier parameter.
// It will not just check the global symboltable but will first check the 
// symboltable of the current subroutine.
Symbol* Parser::GetSymbol(std::string identifier)
{
	Symbol* symbol;
	//std::unique_ptr<Subroutine> subroutine(compiler->GetSubroutine());
	//unique_ptr gives a problem while deleting its own (normal pointer works)
	Subroutine* subroutine = compiler->GetSubroutine();

	if (!subroutine->isEmpty)
	{
		if (subroutine->HasLocal(identifier))
			symbol = subroutine->GetLocal(identifier);
		else
			symbol = compiler->GetSymbol(identifier);
	}
	else
	{
		symbol = compiler->GetSymbol(identifier);
	}

	return symbol;
}
