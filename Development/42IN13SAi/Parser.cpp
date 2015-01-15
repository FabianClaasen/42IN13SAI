#include "Parser.h"

Parser::Parser(Compiler* compiler) : compiler(compiler)
{
}

Parser::Parser(Compiler* compiler, std::vector<std::shared_ptr<Token>> tokens) : compiler(compiler)
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
			returnType = Token(0, 0, 0, "", MyTokenType::Void, nullptr);
			functionName = Token(0, 0, 0, "main", MyTokenType::MainFunction, nullptr);
		}
		else
		{
			returnType = compiler->GetNext();
			functionName = compiler->GetNext();
		}
        
        if (compiler->GetSubroutineTable()->GetSubroutine(functionName.Value) != nullptr)
        {
            compiler->Diag(ExceptionEnum::err_function_been_defined) << currentToken.Value << currentToken.LineNumber;
            compiler->SkipUntil(MyTokenType::CloseMethod);
            return;
        }

		compiler->Match(MyTokenType::OpenBracket);

		// Set the parameters
		if (currentToken.Type == MyTokenType::MainFunction && compiler->PeekNext()->Type != MyTokenType::CloseBracket)
        {
            compiler->Diag(ExceptionEnum::err_main_parameters) << currentToken.LineNumber;
            compiler->SkipUntil(MyTokenType::CloseBracket);
            return;
        }

		while (compiler->PeekNext()->Type != MyTokenType::CloseBracket)
		{
			if (compiler->PeekNext()->Type == MyTokenType::Separator)
			{
				compiler->GetNext(); // remove Separator so you can add a new parameter
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
                {
                    compiler->Diag(ExceptionEnum::err_parameter_in_use) << parameter.Value << currentToken.LineNumber;
                    compiler->SkipUntil(MyTokenType::CloseBracket);
                }
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
            // Catch function in function exception
            if (compiler->PeekNext()->Type == MyTokenType::Function || compiler->PeekNext()->Type == MyTokenType::MainFunction)
            {
                compiler->Diag(ExceptionEnum::err_function_in_function) << compiler->PeekNext()->LineNumber;
                return;
            }
            
			compiler->ParseStatement();
		}

		// Check if the subroutine is closed correctly
		// And add the subroutine to the subroutine table
		compiler->Match(MyTokenType::CloseMethod);
		compiler->AddSubroutine();
	}
    else
    {
        compiler->Diag(ExceptionEnum::err_expected_ret) << currentToken.LineNumber;
        compiler->SkipUntil(MyTokenType::OpenMethod);
        compiler->SkipUntil(*compiler->GetCurrent().Partner);
        return;
    }
}

void Parser::ParseReturn()
{
    if (compiler->GetSubroutine()->returnType == MyTokenType::Void)
    {
        compiler->Diag(ExceptionEnum::err_unexpected_return) << compiler->PeekNext()->LineNumber;
        compiler->SkipUntil(MyTokenType::EOL);
        return;
    }
    
	compiler->Match(MyTokenType::Return);
	
	std::vector<std::shared_ptr<CompilerNode>> nodeParameters;
	nodeParameters.push_back(ParseExpression());
	
	compiler->Match(MyTokenType::EOL);
	
	std::shared_ptr<CompilerNode> returnNode = std::make_shared<CompilerNode>("$ret", nodeParameters, nullptr, false);
	compiler->GetSubroutine()->AddCompilerNode(returnNode);
}

//Also parse (standard) Arithmetical operations
std::shared_ptr<CompilerNode> Parser::ParseAssignmentStatement(bool forLoop)
{
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
    {
        compiler->Diag(ExceptionEnum::err_expected_identifier) << currentToken.LineNumber;
        compiler->SkipUntil(MyTokenType::EOL);
        return std::make_shared<CompilerNode>();
    }

	// Check if the identifier exists
	if (!newIdentifier)
	{
		Symbol* symbol = GetSymbol(identifier.Value);

        if (symbol == nullptr)
        {
            compiler->Diag(ExceptionEnum::err_unkown_identifier) << identifier.Value << currentToken.LineNumber;
            compiler->SkipUntil(MyTokenType::EOL);
            return std::make_shared<CompilerNode>();
        }
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

		if (!compiler->GetSubroutine()->isEmpty)
		{
			compiler->GetSubroutine()->AddLocal(*identifierSymbol);

			delete identifierSymbol;
			identifierSymbol = nullptr;
		}
		else if (!compiler->HasSymbol(identifierSymbol->name))
		{
			compiler->AddSymbol(*identifierSymbol);

			delete identifierSymbol;
			identifierSymbol = nullptr;
		}
		else
		{
            Symbol sym = *identifierSymbol;
			delete identifierSymbol;
            identifierSymbol = nullptr;
            
            compiler->Diag(ExceptionEnum::err_identifier_in_use) << sym.GetValue() << currentToken.LineNumber;
            compiler->SkipUntil(MyTokenType::EOL);
            return std::shared_ptr<CompilerNode>();
		}
	}

	std::shared_ptr<CompilerNode> id = std::make_shared<CompilerNode>("$identifier", identifier.Value, nullptr);
	nodeParameters.push_back(id);

	// Check if it is an assignment or only a decleration.
	if (compiler->PeekNext()->Type == MyTokenType::Equals)
	{
		compiler->GetNext(); // remove the '=' token

		if (compiler->PeekNext()->Value.find(".") != std::string::npos)
		{
			std::string before = compiler->PeekNext()->Value.substr(0, compiler->PeekNext()->Value.find("."));
			std::string after = compiler->PeekNext()->Value.substr(compiler->PeekNext()->Value.find("."));

			if (before.size() > 7 || after.size() > 7)
			{
                compiler->Diag(ExceptionEnum::err_float_precision) << compiler->PeekNext()->LineNumber;
			}
		}

		expression = "$assignment";
		std::shared_ptr<CompilerNode> node = ParseExpression();
		nodeParameters.push_back(node);
		endNode = std::make_shared<CompilerNode>(expression, nodeParameters, nullptr, false);
	}
	else if (IsNextTokenUniOp())
	{
		currentToken = compiler->GetNext();
		std::vector<std::shared_ptr<CompilerNode>> parameters;
		switch (currentToken.Type)
		{
		case MyTokenType::UniOperatorPlus:
			parameters.push_back(std::make_shared<CompilerNode>("$getVariable", identifier.Value, false));
			nodeParameters.push_back(std::make_shared<CompilerNode>("$uniPlus", parameters, nullptr, false));
			break;
		case MyTokenType::UniOperatorMinus:
			parameters.push_back(std::make_shared<CompilerNode>("$getVariable", identifier.Value, false));
			nodeParameters.push_back(std::make_shared<CompilerNode>("$uniMin", parameters, nullptr, false));
			
			break;
		}
		expression = "$assignment";
		endNode = std::make_shared<CompilerNode>(expression, nodeParameters, nullptr, false);
	}
	else if (IsNextTokenUniOp())
	{
		expression = "$assignment";
		std::shared_ptr<CompilerNode> node = ParseExpression();
		nodeParameters.push_back(node);
	}
	else if (IsNextTokenAddOp() || IsNextTokenMulOp())
	{
		std::string command;
		std::vector<std::shared_ptr<CompilerNode>> parameters;

		// Set the command
		Token Operator = compiler->GetNext();
		switch (Operator.Type)
		{
		case MyTokenType::OperatorPlus:
			command = "$add";
			break;
		case MyTokenType::OperatorMinus:
			command = "$min";
			break;
		case MyTokenType::OperatorDivide:
			command = "$div";
			break;
		case MyTokenType::OperatorMultiply:
			command = "$mul";
			break;
		}

		// Check if the function is an assignment
		currentToken = compiler->GetNext();
		if (currentToken.Type != MyTokenType::Equals)
        {
            compiler->Diag(ExceptionEnum::err_expected_identifier) << currentToken.LineNumber;
            compiler->SkipUntil(MyTokenType::EOL);
            return std::make_shared<CompilerNode>();
        }

		// Add the parameters to the parameters list
		expression = "$assignment";
		parameters.push_back(std::make_shared<CompilerNode>("$getVariable", identifier.Value, false));
		parameters.push_back(ParseExpression());
		nodeParameters.push_back(std::make_shared<CompilerNode>(CompilerNode(command, parameters, nullptr, false)));
		endNode = std::make_shared<CompilerNode>(expression, nodeParameters, nullptr, false);
	}

	// Check if the code is closed
	if (!forLoop)
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

	std::shared_ptr<CompilerNode> node = std::make_shared<CompilerNode>("$functionName", currentToken.Value, false);

	nodeParameters.push_back(node);

	compiler->Match(MyTokenType::OpenBracket);
	while (compiler->PeekNext()->Type != MyTokenType::CloseBracket)
	{
		if (compiler->PeekNext()->Type == MyTokenType::Separator)
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
	std::shared_ptr<CompilerNode> endNode = std::make_shared<CompilerNode>("$functionCall", nodeParameters, nullptr, false);

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
        compiler->Diag(ExceptionEnum::err_unexpected_keyword) << currentToken.LineNumber;
        compiler->SkipUntil(MyTokenType::CloseMethod);
        return;
    }
    
    compiler->Match(MyTokenType::OpenBracket);
    
    statementNode = ParseExpression();
    
    std::vector<std::shared_ptr<CompilerNode>> params;
    params.push_back(statementNode);
    
    compiler->Match(MyTokenType::CloseBracket);
    compiler->Match(MyTokenType::OpenMethod);
    
    //Make a do nothing compilerNode to jump to if the statement is false
    std::vector<std::string> doNothing;
    std::shared_ptr<CompilerNode> jumpTo = std::make_shared<CompilerNode>("$doNothing", "", false);
    
    //Create the endNode before parsing the statements in the if/else
    endNode = std::make_shared<CompilerNode>("$if", params, jumpTo, false);
    compiler->GetSubroutine()->AddCompilerNode(endNode);

    while (compiler->PeekNext()->Type != MyTokenType::CloseMethod)
    {
        compiler->ParseStatement();
    }
    
    compiler->Match(MyTokenType::CloseMethod);
    
    // Add a do nothing node to jump to if the 'if' has a partner and
    // is finished with the true condition statements
    std::shared_ptr<CompilerNode> finalDoNothing;
    if (hasPartner)
    {
        finalDoNothing = std::make_shared<CompilerNode>("$doNothing", "donothing-end", false);
        
        std::shared_ptr<CompilerNode> trueDoNothing = std::make_shared<CompilerNode>("$doNothing", "donothing-halfway", finalDoNothing, false);
        
        // Add the do nothing for the true statements
        compiler->GetSubroutine()->AddCompilerNode(trueDoNothing);
    }
    
    //Add the donothing to jump to if the condition is false
    compiler->GetSubroutine()->AddCompilerNode(jumpTo);
    
    // If there is a else / else if parse that after the do nothing
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
    
    // Add the end doNothing
    compiler->GetSubroutine()->AddCompilerNode(finalDoNothing);
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
        compiler->Diag(ExceptionEnum::err_unexpected_keyword) << currentToken.LineNumber;
        compiler->SkipUntil(MyTokenType::CloseMethod);
        return;
	}

	compiler->Match(MyTokenType::OpenBracket);

	if (forLoop)
	{
		nodeParameters.push_back(ParseAssignmentStatement(true));
		compiler->Match(MyTokenType::Separator);
		nodeParameters.push_back(ParseExpression());
		compiler->Match(MyTokenType::Separator);

		// Check if it is a uni operator, if true it should parse an assign
		if (compiler->PeekNext()->Type == MyTokenType::Identifier)
			nodeParameters.push_back(ParseAssignmentStatement(true));
		else
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
	std::shared_ptr<CompilerNode> jumpTo = std::make_shared<CompilerNode>("$doNothing", "", false);
	
	//Make the endNode before parsing the statements in the loop
	endNode = std::make_shared<CompilerNode>(statementExpression, nodeParameters, jumpTo, false);
	compiler->GetSubroutine()->AddCompilerNode(endNode);
	
	// set the doNothing jumpto to the endNode so it can jump to it at the end of the while
	jumpTo->SetJumpTo(endNode);
	
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
			parsedExpr = std::make_shared<CompilerNode>("$and", parameters, nullptr, false);
			break;
		case MyTokenType::Or:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = std::make_shared<CompilerNode>("$or", parameters, nullptr, false);
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
			parsedExpr = std::make_shared<CompilerNode>(CompilerNode("$less", parameters, nullptr, false));
			break;
		case MyTokenType::LowerOrEqThan:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = std::make_shared<CompilerNode>(CompilerNode("$lessOrEq", parameters, nullptr, false));
			break;
		case MyTokenType::GreaterThan:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = std::make_shared<CompilerNode>(CompilerNode("$greater", parameters, nullptr, false));
			break;
		case MyTokenType::GreaterOrEqThan:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			parsedExpr = std::make_shared<CompilerNode>(CompilerNode("$greaterOrEq", parameters, nullptr, false));
			break;
		case MyTokenType::Comparator:
			parameters.push_back(parsedExpr);
			parameters.push_back(secondParsedExpr);
			if (relOp.Value == "==")
				parsedExpr = std::make_shared<CompilerNode>(CompilerNode("$equals", parameters, nullptr, false));
			else
				parsedExpr = std::make_shared<CompilerNode>(CompilerNode("$notEquals", parameters, nullptr, false));
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

		if (secondParsedExpr != nullptr)
		{
			switch (addOp.Type)
			{
			case MyTokenType::OperatorPlus:
				parameters.push_back(parsedExpr);
				parameters.push_back(secondParsedExpr);
				parsedExpr = std::make_shared<CompilerNode>(CompilerNode("$add", parameters, nullptr, false));
				break;
			case MyTokenType::OperatorMinus:
				parameters.push_back(parsedExpr);
				parameters.push_back(secondParsedExpr);
				parsedExpr = std::make_shared<CompilerNode>(CompilerNode("$min", parameters, nullptr, false));
				break;
			}
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

		if (secondTerm != nullptr)
		{
			switch (mullOp.Type)
			{
			case MyTokenType::OperatorMultiply:
				parameters.push_back(term);
				parameters.push_back(secondTerm);
				term = std::make_shared<CompilerNode>("$mul", parameters, nullptr, false);
				break;
			case MyTokenType::OperatorDivide:
				parameters.push_back(term);
				parameters.push_back(secondTerm);
				if (secondTerm->GetValue() == "0")
				{
                    compiler->Diag(ExceptionEnum::err_zero_divide) << mullOp.LineNumber;
                    secondTerm = nullptr;
				}
				term = std::make_shared<CompilerNode>("$div", parameters, nullptr, false);
				break;
			case MyTokenType::OperatorRaised:
				parameters.push_back(term);
				parameters.push_back(secondTerm);
				term = std::make_shared<CompilerNode>("$raise", parameters, nullptr, false);
				break;
			}
		}
	}
	return term;
}

std::shared_ptr<CompilerNode> Parser::ParseUniExpression()
{
	std::shared_ptr<CompilerNode> term = ParseTerm();

	while (IsNextTokenUniOp())
	{
		if (term != nullptr)
		{
			Token uniOp = compiler->GetNext();
			std::vector<std::shared_ptr<CompilerNode>> parameters;

			switch (uniOp.Type)
			{
			case MyTokenType::UniOperatorPlus:
				parameters.push_back(term);
				term = std::make_shared<CompilerNode>("$uniPlus", parameters, nullptr, false);
				if (compiler->PeekNext()->Type != MyTokenType::CloseBracket)
					compiler->Match(MyTokenType::EOL);
				break;
			case MyTokenType::UniOperatorMinus:
				parameters.push_back(term);
				term = std::make_shared<CompilerNode>("$uniMin", parameters, nullptr, false);
				if (compiler->PeekNext()->Type != MyTokenType::CloseBracket)
					compiler->Match(MyTokenType::EOL);
				break;
			}
		}
	}

	return term;
}

std::shared_ptr<CompilerNode> Parser::ParseTerm()
{
	Token token = *compiler->PeekNext();

	std::shared_ptr<CompilerNode> node = nullptr;

	if (token.Type == MyTokenType::Float)
	{
        token = compiler->GetNext();
		node = std::make_shared<CompilerNode>("$value", token.Value, false);
		return node;
	}
	else if (token.Type == MyTokenType::Identifier)
	{
        token = compiler->GetNext();
		if (compiler->PeekNext()->Type == MyTokenType::OpenBracket)
		{
			return ParseFunctionCall(token);
		}
		else
		{
			std::string identifier = token.Value;

			Symbol* symbol = GetSymbol(identifier);

            if (symbol == nullptr)
            {
                compiler->Diag(ExceptionEnum::err_var_not_found) << identifier << token.LineNumber;
                symbol = new Symbol("fault", MyTokenType::Identifier, SymbolKind::None);
            }

			node = std::make_shared<CompilerNode>("$getVariable", symbol->name, false);
			return node;
		}
	}
	else if (token.Type == MyTokenType::OpenBracket)
	{
        token = compiler->GetNext();
		node = ParseExpression();
		compiler->Match(MyTokenType::CloseBracket);
		return node;
	}
	else if (token.Type == MyTokenType::OperatorMinus && compiler->PeekNext()->Type == MyTokenType::Float)
	{
        compiler->GetNext();
		token = compiler->GetNext();
		node = std::make_shared<CompilerNode>("$value", "-" + token.Value, false);
	}
	else if (compiler->IsInternalFunction(token.Type) && !(token.Type==MyTokenType::PrintLine || token.Type==MyTokenType::Stop))
	{
        token = compiler->GetNext();
		node = std::shared_ptr<CompilerNode>(InternalFunction(compiler).GetInternalFunction(token.Type));
			return node;
	}
	else if (compiler->IsConstant(token.Type))
	{
        token = compiler->GetNext();
		node = std::shared_ptr<CompilerNode>(Constant(compiler).GetConstant(token.Type));
			return node;
	}
	else if (compiler->PeekNext()->Type == MyTokenType::OpenBracket)
	{
        token = compiler->GetNext();
		return ParseFunctionCall(token);
	}
    else if (token.Type == MyTokenType::Return)
    {
        token = compiler->GetNext();
        compiler->Diag(ExceptionEnum::err_ret_as_var) << token.LineNumber;
    }

	return node;
}

// This function is called when there is a function inside a expression
std::shared_ptr<CompilerNode> Parser::ParseFunctionCall(Token token)
{
	std::vector<std::shared_ptr<CompilerNode>> nodeParameters;
	std::shared_ptr<CompilerNode> node = std::make_shared<CompilerNode>("$functionName", token.Value, false);
	nodeParameters.push_back(node);
	Token currentToken = compiler->GetNext();

	while (compiler->PeekNext()->Type != MyTokenType::CloseBracket)
	{
		if (compiler->PeekNext()->Type == MyTokenType::Separator)
		{
			currentToken = compiler->GetNext();
		}

		std::shared_ptr<CompilerNode> node = ParseExpression();
		nodeParameters.push_back(node);
	}

	compiler->Match(MyTokenType::CloseBracket);

	std::shared_ptr<CompilerNode> finalNode = std::make_shared<CompilerNode>("$functionCall", nodeParameters, nullptr, false);
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
