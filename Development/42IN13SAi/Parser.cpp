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

    if (currentToken.Type == MyTokenType::MainFunction || IsNextTokenReturnType())
	{
        Token returnType;
        Token functionName;
        if (currentToken.Type == MyTokenType::MainFunction)
        {
            returnType = Token(0,0,0, "", MyTokenType::Void, nullptr);
            functionName = Token(0,0,0, "main", MyTokenType::MainFunction, nullptr);
        }
        else
        {
            returnType = compiler->GetNext();
            functionName = compiler->GetNext();
        }

		compiler->Match(MyTokenType::OpenBracket);

		// Set the parameters
		while (compiler->PeekNext()->Type != MyTokenType::CloseBracket)
		{
			if (compiler->PeekNext()->Type == MyTokenType::Seperator)
			{
				compiler->GetNext(); // remove seperator so you can add a new parameter
			}
			else
			{
				Token parameter = compiler->GetNext();
                SymbolKind kind = SymbolKind::Parameter;
                if (IsTokenReturnVarType(parameter))
                    kind = SymbolKind::ParameterType;
                
				Symbol parameterSymbol = Symbol(parameter.Value, parameter.Type, kind);
				

				if (!symbolTable.HasSymbol(parameterSymbol.name))
				{
					symbolTable.AddSymbol(parameterSymbol);
				}
				else
					throw std::runtime_error("Parameter name is already in use");
			}
		}
        
        if (compiler->PeekNext()->Type == MyTokenType::CloseBracket)
            compiler->GetNext();

		// Check if the functions starts and create a subroutine
		compiler->Match(MyTokenType::OpenMethod);
		compiler->SetSubroutine(Subroutine(functionName.Value, returnType.Type, SubroutineKind::Function, symbolTable));

		// Set all the statements inside this subroutine
		while (compiler->PeekNext()->Type != MyTokenType::CloseMethod && compiler->PeekNext()->Level > 1)
		{
			compiler->ParseStatement();
		}

		// Check if the subroutine is closed correctly
		// And add the subroutine to the subroutine table
		compiler->Match(MyTokenType::CloseMethod);
		compiler->AddSubroutine();
	}
	else
		throw std::runtime_error("Expected return type");
}

void Parser::ParseReturn()
{
    compiler->Match(MyTokenType::Return);
    
	std::vector<std::shared_ptr<CompilerNode>> nodeParameters;
    nodeParameters.push_back(ParseExpression());
    
    compiler->Match(MyTokenType::EOL);
    
	std::shared_ptr<CompilerNode> returnNode(new CompilerNode("$ret", nodeParameters, nullptr, false));
    compiler->GetSubroutine()->AddCompilerNode(returnNode);
}

//Also parse (standard) Arithmetical operations
std::shared_ptr<CompilerNode> Parser::ParseAssignmentStatement(bool forLoop)
{
#ifdef _WIN32
	// Memory leaks notifier
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	std::string expression = "";
	std::vector<std::shared_ptr<CompilerNode>> nodeParameters;
	std::shared_ptr<CompilerNode> endNode = nullptr;
	Subroutine* subroutine = compiler->GetSubroutine();

	bool newIdentifier = false;

	Token identifier;
	Token currentToken = compiler->GetNext();
	if (currentToken.Type == MyTokenType::Var)
	{
		newIdentifier = true;
		identifier = compiler->GetNext();
	}
	else
	{
		identifier = currentToken;
	}

	// Check if the identifier is a identifier
	if (identifier.Type != MyTokenType::Identifier)
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

	std::shared_ptr<CompilerNode> id(new CompilerNode("$identifier", identifier.Value, nullptr));
	nodeParameters.push_back(id);

	// Check if it is an assignment or only a decleration.
	if (compiler->PeekNext()->Type == MyTokenType::Equals)
	{
		compiler->GetNext(); // remove the '=' token
		expression = "$assignment";
		std::shared_ptr<CompilerNode> node = ParseExpression();
		nodeParameters.push_back(node);
		endNode = std::shared_ptr<CompilerNode>(new CompilerNode(expression, nodeParameters, nullptr, false));
	}
	else if (IsNextTokenUniOp())
	{
		currentToken = compiler->GetNext();
		std::vector<std::shared_ptr<CompilerNode>> parameters;
		switch (currentToken.Type)
		{
		case MyTokenType::UniOperatorPlus:
			parameters.push_back(std::shared_ptr<CompilerNode>(new CompilerNode("$getVariable", identifier.Value, false)));
			nodeParameters.push_back(std::shared_ptr<CompilerNode>(new CompilerNode("$uniPlus", parameters, nullptr, false)));
			break;
		case MyTokenType::UniOperatorMinus:
			parameters.push_back(std::shared_ptr<CompilerNode>(new CompilerNode("$getVariable", identifier.Value, false)));
			nodeParameters.push_back(std::shared_ptr<CompilerNode>(new CompilerNode("$uniMin", parameters, nullptr, false)));
            
			break;
		}
        expression = "$assignment";
		endNode = std::shared_ptr<CompilerNode>(new CompilerNode(expression, nodeParameters, nullptr, false));
	}
    else if (IsNextTokenUniOp())
    {
        expression = "$assignment";
		std::shared_ptr<CompilerNode> node = ParseExpression();
        nodeParameters.push_back(node);
    }

	// Check if the code is closed
	compiler->Match(MyTokenType::EOL);

	if (!forLoop && endNode != nullptr)
	{
		if (compiler->GetSubroutine()->isEmpty)
			compiler->AddCompilerNode(endNode);
		else
			compiler->GetSubroutine()->AddCompilerNode(endNode);
	}

	return endNode;
}

// This function is only called when a function is called without it being in a assignment.
void Parser::ParseFunctionCall()
{
	std::vector<std::shared_ptr<CompilerNode>> nodeParameters;
	Token currentToken = compiler->GetNext();

	std::shared_ptr<CompilerNode> node(new CompilerNode("$functionName", currentToken.Value, false));
	nodeParameters.push_back(node);

    compiler->Match(MyTokenType::OpenBracket);
	while (compiler->PeekNext()->Type != MyTokenType::CloseBracket)
	{
		if (compiler->PeekNext()->Type == MyTokenType::Seperator)
		{
			currentToken = compiler->GetNext();
		}

		std::shared_ptr<CompilerNode> node = ParseExpression();
        if (node != nullptr)
            nodeParameters.push_back(node);
	}

	compiler->Match(MyTokenType::CloseBracket);
	compiler->Match(MyTokenType::EOL);

	// Create the finall node
	std::shared_ptr<CompilerNode> endNode(new CompilerNode("$functionCall", nodeParameters, nullptr, false));

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

	std::list<CompilerNode> innerIfStatementNodes;
	std::list<CompilerNode> innerElseStatementNodes;
	std::shared_ptr<CompilerNode> statementNode;
	std::shared_ptr<CompilerNode> endNode;

	if (currentToken.Type == MyTokenType::If)
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

	compiler->Match(MyTokenType::OpenBracket);

	statementNode = ParseExpression();

	compiler->Match(MyTokenType::CloseBracket);
	compiler->Match(MyTokenType::OpenMethod);

	//Make a do nothing compilerNode to jump to if the statement is false
	std::vector<std::string> doNothing;
	std::shared_ptr<CompilerNode> jumpTo(new CompilerNode("$doNothing", "", false));
	statementNode->SetJumpTo(jumpTo);

	//Create the endNode before parsing the statements in the if/else
	std::vector<std::shared_ptr<CompilerNode>> params;
	params.push_back(statementNode);
	endNode = std::shared_ptr<CompilerNode>(new CompilerNode("$if", params, nullptr, false));
	compiler->AddCompilerNode(endNode);

	while (compiler->PeekNext()->Type != MyTokenType::CloseMethod)
	{
		compiler->ParseStatement();
	}

	compiler->Match(MyTokenType::CloseMethod);

	if (hasPartner)
	{
		compiler->Match(MyTokenType::Else);
		compiler->Match(MyTokenType::OpenMethod);

		while (compiler->PeekNext()->Type != MyTokenType::CloseMethod)
		{
			compiler->ParseStatement();
		}

		compiler->Match(MyTokenType::CloseMethod);
	}

	//Finally add the do nothing CompilerNode to jump to if the statement is false
	compiler->AddCompilerNode(jumpTo);
}

/*
Parse while and for loops
*/
void Parser::ParseLoopStatement()
{
	Token currentToken = compiler->GetNext();
	bool forLoop = false;

	std::vector<std::shared_ptr<CompilerNode>> nodeParameters;
	std::string statementExpression;

	std::shared_ptr<CompilerNode> endNode;
	std::list<CompilerNode> innerStatementNodes;

	if (currentToken.Type == MyTokenType::While || currentToken.Type == MyTokenType::ForLoop)
	{
		if (currentToken.Type == MyTokenType::ForLoop)
		{
			forLoop = true;
		}
	}
	else
	{
		throw std::runtime_error("Expected a loop keyword");
	}

	compiler->Match(MyTokenType::OpenBracket);

	if (forLoop)
	{
		nodeParameters.push_back(ParseAssignmentStatement(true));
		nodeParameters.push_back(ParseExpression());
		compiler->GetNext();
		nodeParameters.push_back(ParseExpression());

		statementExpression = "$forLoop";
	}
	else
	{
		nodeParameters.push_back(ParseExpression());

		statementExpression = "$whileLoop";
    }
    
    // Create a do nothing, so you can jump to this when the statement is false
    std::vector<std::string> doNothing;
    std::shared_ptr<CompilerNode> jumpTo(new CompilerNode("$doNothing", "", false));
    
    //Make the endNode before parsing the statements in the loop
	endNode = std::shared_ptr<CompilerNode>(new CompilerNode(statementExpression, nodeParameters, jumpTo, false));
    compiler->GetSubroutine()->AddCompilerNode(endNode);
    
    compiler->Match(MyTokenType::CloseBracket);
    compiler->Match(MyTokenType::OpenMethod);
    
    while (compiler->PeekNext()->Type != MyTokenType::CloseMethod)
    {
        compiler->ParseStatement();
    }
    
    compiler->Match(MyTokenType::CloseMethod);
    
    //Finally add the jumpTo compilerNode
    
    compiler->GetSubroutine()->AddCompilerNode(jumpTo);
}

#pragma endregion ParseStatementMethods

#pragma region ParseExpressionMethods
std::shared_ptr<CompilerNode> Parser::ParseExpression()
{
	std::shared_ptr<CompilerNode> parsedExpr = ParseRelationalExpression();
	while (IsNextTokenLogicalOp())
	{
		Token logicalOp = compiler->GetNext();
		std::shared_ptr<CompilerNode> secondParsedExpr = ParseRelationalExpression();
		std::vector<std::shared_ptr<CompilerNode>> parameters;

		switch (logicalOp.Type)
		{
		case MyTokenType::And:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = std::shared_ptr<CompilerNode>(new CompilerNode("$and", parameters, nullptr, false));
			break;
		case MyTokenType::Or:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = std::shared_ptr<CompilerNode>(new CompilerNode("$or", parameters, nullptr, false));
			break;
		}
	}

	return parsedExpr;
}

std::shared_ptr<CompilerNode> Parser::ParseRelationalExpression()
{
	std::shared_ptr<CompilerNode> parsedExpr = ParseAddExpression();
	while (IsNextTokenRelationalOp())
	{
		Token relOp = compiler->GetNext();
		std::shared_ptr<CompilerNode> secondParsedExpr = ParseAddExpression();
		std::vector<std::shared_ptr<CompilerNode>> parameters;

		switch (relOp.Type)
		{
		case MyTokenType::LowerThan:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = std::shared_ptr<CompilerNode>(new CompilerNode("$less", parameters, nullptr, false));
			break;
		case MyTokenType::LowerOrEqThan:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = std::shared_ptr<CompilerNode>(new CompilerNode("$lessOrEq", parameters, nullptr, false));
			break;
		case MyTokenType::GreaterThan:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = std::shared_ptr<CompilerNode>(new CompilerNode("$greater", parameters, nullptr, false));
			break;
		case MyTokenType::GreaterOrEqThan:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = std::shared_ptr<CompilerNode>(new CompilerNode("$greaterOrEq", parameters, nullptr, false));
			break;
		case MyTokenType::Comparator:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = std::shared_ptr<CompilerNode>(new CompilerNode("$equals", parameters, nullptr, false));
			break;
		}
	}

	return parsedExpr;
}

std::shared_ptr<CompilerNode> Parser::ParseAddExpression()
{
	std::shared_ptr<CompilerNode> parsedExpr = ParseMulExpression();
	while (IsNextTokenAddOp())
	{
		Token addOp = compiler->GetNext();
		std::shared_ptr<CompilerNode> secondParsedExpr = ParseMulExpression();
		std::vector<std::shared_ptr<CompilerNode>> parameters;

		switch (addOp.Type)
		{
		case MyTokenType::OperatorPlus:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = std::shared_ptr<CompilerNode>(new CompilerNode("$add", parameters, nullptr, false));
			break;
		case MyTokenType::OperatorMinus:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = std::shared_ptr<CompilerNode>(new CompilerNode("$min", parameters, nullptr, false));
			break;
		}
	}

	return parsedExpr;
}

std::shared_ptr<CompilerNode> Parser::ParseMulExpression()
{
	std::shared_ptr<CompilerNode> term = ParseUniExpression();
	while (IsNextTokenMulOp())
	{
		Token mullOp = compiler->GetNext();
		std::shared_ptr<CompilerNode> secondTerm = ParseUniExpression();
		std::vector<std::shared_ptr<CompilerNode>> parameters;

		switch (mullOp.Type)
		{
		case MyTokenType::OperatorMultiply:
			parameters.push_back(term);
			parameters.push_back(secondTerm);
			term = std::shared_ptr<CompilerNode>(new CompilerNode("$mul", parameters, nullptr, false));
			break;
		case MyTokenType::OperatorDivide:
			parameters.push_back(term);
			parameters.push_back(secondTerm);
			term = std::shared_ptr<CompilerNode>(new CompilerNode("$div", parameters, nullptr, false));
			break;
		case MyTokenType::OperatorRaised:
			parameters.push_back(term);
			parameters.push_back(secondTerm);
			term = std::shared_ptr<CompilerNode>(new CompilerNode("$raise", parameters, nullptr, false));
			break;
		}
	}

	return term;
}

std::shared_ptr<CompilerNode> Parser::ParseUniExpression()
{
	std::shared_ptr<CompilerNode> term = ParseTerm();

	while (IsNextTokenUniOp())
	{
		Token uniOp = compiler->GetNext();
		std::vector<std::shared_ptr<CompilerNode>> parameters;

		switch (uniOp.Type)
		{
		case MyTokenType::UniOperatorPlus:
			parameters.push_back(term);
			term = std::shared_ptr<CompilerNode>(new CompilerNode("$uniPlus", parameters, nullptr, false));
			compiler->Match(MyTokenType::EOL);
			break;
        case MyTokenType::UniOperatorMinus:
			parameters.push_back(term);
			term = std::shared_ptr<CompilerNode>(new CompilerNode("$uniMin", parameters, nullptr, false));
			compiler->Match(MyTokenType::EOL);
			break;
		}
	}

	return term;
}

std::shared_ptr<CompilerNode> Parser::ParseTerm()
{
	Token token = compiler->GetNext();

	std::shared_ptr<CompilerNode> node = nullptr;

	if (token.Type == MyTokenType::Float)
	{
		node = std::shared_ptr<CompilerNode>(new CompilerNode("$value", token.Value, false));
		return node;
	}
	else if (token.Type == MyTokenType::Identifier)
	{
		if (compiler->PeekNext()->Type == MyTokenType::OpenBracket)
		{
			return ParseFunctionCall(token);
		}
		else
		{
			std::string identifier = token.Value;

			Symbol* symbol = GetSymbol(identifier);

			if (symbol == nullptr)
				throw SymbolNotFoundException("");

			node = std::shared_ptr<CompilerNode>(new CompilerNode("$getVariable", symbol->name, false));
			return node;
		}
	}
	else if (token.Type == MyTokenType::OpenBracket)
	{
		node = ParseExpression();
		compiler->Match(MyTokenType::CloseBracket);
		return node;
	}
	else if (compiler->IsInternalFunction(token.Type) && !(token.Type==MyTokenType::PrintLine || token.Type==MyTokenType::Stop))
	{
		node = std::shared_ptr<CompilerNode>(InternalFunction(compiler).GetInternalFunction(token.Type));
			return node;
	}
    else if (compiler->PeekNext()->Type == MyTokenType::OpenBracket)
    {
        return ParseFunctionCall(token);
    }
	return node;
}

// This function is called when there is a function inside a expression
std::shared_ptr<CompilerNode> Parser::ParseFunctionCall(Token token)
{
	std::vector<std::shared_ptr<CompilerNode>> nodeParameters;
	std::shared_ptr<CompilerNode> node(new CompilerNode("$functionName", token.Value, false));
	nodeParameters.push_back(node);
	Token currentToken = compiler->GetNext();

	while (compiler->PeekNext()->Type != MyTokenType::CloseBracket)
	{
		if (compiler->PeekNext()->Type == MyTokenType::Seperator)
		{
			currentToken = compiler->GetNext();
		}

		std::shared_ptr<CompilerNode> node = ParseExpression();
		nodeParameters.push_back(node);
	}

	compiler->Match(MyTokenType::CloseBracket);

	std::shared_ptr<CompilerNode> finalNode(new CompilerNode("$functionCall", nodeParameters, nullptr, false));
	return finalNode;
}
#pragma endregion ParseExpressionMethods

#pragma region IsNextTokenMethods
bool Parser::IsNextTokenLogicalOp()
{
	MyTokenType type = compiler->PeekNext()->Type;
	return type == MyTokenType::And ||
		type == MyTokenType::Or;

	/*std::vector<MyTokenType> operators{ MyTokenType::And, MyTokenType::Or };
	return std::find(operators.begin(), operators.end(), compiler->PeekNext()->Type) != operators.end();*/
}

bool Parser::IsNextTokenRelationalOp()
{
	MyTokenType type = compiler->PeekNext()->Type;
	return type == MyTokenType::GreaterThan ||
		type == MyTokenType::GreaterOrEqThan ||
		type == MyTokenType::LowerThan ||
		type == MyTokenType::LowerOrEqThan ||
		type == MyTokenType::Comparator;

	/*std::vector<MyTokenType> operators{ MyTokenType::GreaterThan, MyTokenType::GreaterOrEqThan, MyTokenType::LowerThan, MyTokenType::LowerOrEqThan };
	return std::find(operators.begin(), operators.end(), compiler->PeekNext()->Type) != operators.end();*/
}

bool Parser::IsNextTokenAddOp()
{
	MyTokenType type = compiler->PeekNext()->Type;
	return type == MyTokenType::OperatorMinus ||
		type == MyTokenType::OperatorPlus;

	/*std::vector<MyTokenType> operators{ MyTokenType::OperatorPlus, MyTokenType::OperatorMinus };
	return std::find(operators.begin(), operators.end(), compiler->PeekNext()->Type) != operators.end();*/
}

bool Parser::IsNextTokenMulOp()
{
	MyTokenType type = compiler->PeekNext()->Type;
	return type == MyTokenType::OperatorMultiply ||
		type == MyTokenType::OperatorDivide ||
		type == MyTokenType::OperatorRaised;

	/*std::vector<MyTokenType> operators{ MyTokenType::OperatorMultiply, MyTokenType::OperatorDivide, MyTokenType::OperatorRaised };
	return std::find(operators.begin(), operators.end(), compiler->PeekNext()->Type) != operators.end();*/
}

bool Parser::IsNextTokenUniOp()
{
	MyTokenType type = compiler->PeekNext()->Type;
	return type == MyTokenType::UniOperatorPlus ||
		type == MyTokenType::UniOperatorMinus;

	/*std::vector<MyTokenType> operators{ MyTokenType::UniOperatorPlus, MyTokenType::UniOperatorMinus };
	return std::find(operators.begin(), operators.end(), compiler->PeekNext()->Type) != operators.end();*/
}

bool Parser::IsNextTokenReturnType()
{
	MyTokenType type = compiler->PeekNext()->Type;
	return type == MyTokenType::Void ||
		type == MyTokenType::None ||
		type == MyTokenType::FloatReturn;

	/*std::vector<MyTokenType> operators{ MyTokenType::Void, MyTokenType::None, MyTokenType::Float };
	return std::find(operators.begin(), operators.end(), compiler->PeekNext()->Type) != operators.end();*/
}
#pragma endregion IsNextTokenMethods

bool Parser::IsTokenReturnVarType(Token token)
{
    return token.Type == MyTokenType::FloatReturn;
}

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
