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
    
    std::vector<CompilerNode*> nodeParameters;
    nodeParameters.push_back(ParseExpression());
    
    compiler->Match(MyTokenType::EOL);
    
    CompilerNode returnNode = CompilerNode("$ret", nodeParameters, nullptr, false);
    compiler->GetSubroutine()->AddCompilerNode(returnNode);
}

//Also parse (standard) Arithmetical operations
CompilerNode* Parser::ParseAssignmentStatement(bool forLoop)
{
	std::string expression = "";
	std::vector<CompilerNode*> nodeParameters;
	CompilerNode* endNode = nullptr;
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

	CompilerNode* id = new CompilerNode("$identifier", identifier.Value, nullptr);
	nodeParameters.push_back(id);

	// Check if it is an assignment or only a decleration.
	if (compiler->PeekNext()->Type == MyTokenType::Equals)
	{
		compiler->GetNext(); // remove the '=' token
		expression = "$assignment";
		CompilerNode* node = ParseExpression();
		nodeParameters.push_back(node);
		endNode = new CompilerNode(expression, nodeParameters, nullptr, false);
	}
	else if (IsNextTokenUniOp())
	{
		currentToken = compiler->GetNext();
		std::vector<CompilerNode*> parameters;
		switch (currentToken.Type)
		{
		case MyTokenType::UniOperatorPlus:
			parameters.push_back(new CompilerNode("$identifier", identifier.Value, false));
			endNode = new CompilerNode("$uniPlus", parameters, nullptr, false);
			break;
		case MyTokenType::UniOperatorMinus:
			parameters.push_back(new CompilerNode("$identifier", identifier.Value, false));
			endNode = new CompilerNode("$uniMin", parameters, nullptr, false);
			break;
		}
	}
    else if (IsNextTokenUniOp())
    {
        expression = "$assignment";
        CompilerNode* node = ParseExpression();
        nodeParameters.push_back(node);
    }

	// Check if the code is closed
	compiler->Match(MyTokenType::EOL);

	if (!forLoop)
	{
		if (compiler->GetSubroutine()->isEmpty)
			compiler->AddCompilerNode(*endNode);
		else
			compiler->GetSubroutine()->AddCompilerNode(*endNode);
	}

	return endNode;
}

// This function is only called when a function is called without it being in a assignment.
void Parser::ParseFunctionCall()
{
	std::vector<CompilerNode*> nodeParameters;
	Token currentToken = compiler->GetNext();

	CompilerNode* node = new CompilerNode("$functionName", currentToken.Value, false);
	nodeParameters.push_back(node);

    compiler->Match(MyTokenType::OpenBracket);
	while (compiler->PeekNext()->Type != MyTokenType::CloseBracket)
	{
		currentToken = compiler->GetNext();
		if (currentToken.Type == MyTokenType::Seperator)
		{
			currentToken = compiler->GetNext();
		}

		CompilerNode* node = ParseExpression();
        if (node != nullptr)
            nodeParameters.push_back(node);
	}

	compiler->Match(MyTokenType::CloseBracket);
	compiler->Match(MyTokenType::EOL);

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

	std::list<CompilerNode> innerIfStatementNodes;
	std::list<CompilerNode> innerElseStatementNodes;
	CompilerNode* statementNode;
	CompilerNode endNode;

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
	CompilerNode jumpTo = CompilerNode("$doNothing", "", false);
	statementNode->SetJumpTo(jumpTo);

	//Create the endNode before parsing the statements in the if/else
	std::vector<CompilerNode*> params;
	params.push_back(statementNode);
	endNode = CompilerNode("$if", params, nullptr, false);
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

	std::vector<CompilerNode*> nodeParameters;
	std::string statementExpression;

	CompilerNode endNode;
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
    CompilerNode jumpTo = CompilerNode("$doNothing", "", false);
    
    //Make the endNode before parsing the statements in the loop
    endNode = CompilerNode(statementExpression, nodeParameters, &jumpTo, false);
    compiler->AddCompilerNode(endNode);
    
    compiler->Match(MyTokenType::CloseBracket);
    compiler->Match(MyTokenType::OpenMethod);
    
    while (compiler->PeekNext()->Type != MyTokenType::CloseMethod)
    {
        compiler->ParseStatement();
    }
    
    compiler->Match(MyTokenType::CloseMethod);
    
    //Finally add the jumpTo compilerNode
    
    compiler->AddCompilerNode(jumpTo);
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
		case MyTokenType::And:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = new CompilerNode("$and", parameters, nullptr, false);
			break;
		case MyTokenType::Or:
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
		case MyTokenType::LowerThan:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = new CompilerNode("$less", parameters, nullptr, false);
			break;
		case MyTokenType::LowerOrEqThan:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = new CompilerNode("$lessOrEq", parameters, nullptr, false);
			break;
		case MyTokenType::GreaterThan:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = new CompilerNode("$greater", parameters, nullptr, false);
			break;
		case MyTokenType::GreaterOrEqThan:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = new CompilerNode("$greaterOrEq", parameters, nullptr, false);
			break;
		case MyTokenType::Comparator:
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
		case MyTokenType::OperatorPlus:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = new CompilerNode("$add", parameters, nullptr, false);
			break;
		case MyTokenType::OperatorMinus:
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
		case MyTokenType::OperatorMultiply:
			parameters.push_back(term);
			parameters.push_back(secondTerm);
			term = new CompilerNode("$mul", parameters, nullptr, false);
			break;
		case MyTokenType::OperatorDivide:
			parameters.push_back(term);
			parameters.push_back(secondTerm);
			term = new CompilerNode("$div", parameters, nullptr, false);
			break;
		case MyTokenType::OperatorRaised:
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
		case MyTokenType::UniOperatorPlus:
			parameters.push_back(term);
			term = new CompilerNode("$uniPlus", parameters, nullptr, false);
			compiler->Match(MyTokenType::EOL);
			break;
        case MyTokenType::UniOperatorMinus:
			parameters.push_back(term);
			term = new CompilerNode("$uniMin", parameters, nullptr, false);
			compiler->Match(MyTokenType::EOL);
			break;
		}
	}

	return term;
}

CompilerNode* Parser::ParseTerm()
{
	Token token = compiler->GetNext();

	CompilerNode* node = nullptr;

	if (token.Type == MyTokenType::Float)
	{
		node = new CompilerNode("$value", token.Value, false);
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

			node = new CompilerNode("$getVariable", symbol->name, false);
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
			node = InternalFunction(compiler).GetInternalFunction(token.Type);
			return node;
	}
    else if (compiler->PeekNext()->Type == MyTokenType::OpenBracket)
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

	while (compiler->PeekNext()->Type != MyTokenType::CloseBracket)
	{
		if (compiler->PeekNext()->Type == MyTokenType::Seperator)
		{
			currentToken = compiler->GetNext();
		}

		CompilerNode* node = ParseExpression();
		nodeParameters.push_back(node);
	}

	compiler->Match(MyTokenType::CloseBracket);

	CompilerNode* finalNode = new CompilerNode("$functionCall", nodeParameters, nullptr, false);
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
