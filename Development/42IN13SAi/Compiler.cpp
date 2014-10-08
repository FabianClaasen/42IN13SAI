#include "Compiler.h"
#include "CompilerNode.h"
#include "Token.h"

#include <iterator>
#include <string>
#include <list>


/*
Constructor
*/
Compiler::Compiler()
{

}

Compiler::Compiler(std::list<Token> tokens)
{
	tokenizerTokens = tokens;

	Compile();
}

/*
Destructor
*/
Compiler::~Compiler()
{
}

/*
keep parsing as long as there are tokens
*/
void Compiler::Compile()
{
	while (Compiler::PeekNext() != nullptr)
	{
		parser.ParseStatement();
	}
}

/*
 Check what the next token is
 */
Token* Compiler::PeekNext()
{
    std::list<Token>::iterator it = tokenizerTokens.begin();
    std::advance(it, currentToken + 1);
    
    return &*it;
}

/*
 Get the next token
 */
Token Compiler::GetNext()
{
    std::list<Token>::iterator it = tokenizerTokens.begin();
    
    if (Compiler::PeekNext() != nullptr)
    {
        std::advance(it, ++currentToken);
    }
    else
    {
        throw std::runtime_error("Token missing");
    }
    
    return *it;
}

void Compiler::Match(TokenType type)
{
<<<<<<< HEAD
	std::string expression;
	std::string identifier;
	std::string value;

	Token currentToken = getNext();
	if (currentToken.Type == TokenType::Identifier)
	{
		identifier = currentToken.Value;
	}
	else
	{
		throw new exception("Identifier expected");
	}
	
	currentToken = getNext();

	/*if (currentToken.Type == TokenType::OperatorDivide || currentToken.Type == TokenType::OperatorMinus || currentToken.Type == TokenType::OperatorMultiply || currentToken.Type == TokenType::OperatorPlus || currentToken.Type == TokenType::OperatorRaised)
	{
		parseOperatorStatement();
	}
	else if (currentToken.Type == TokenType::UniOperatorMinus || currentToken.Type == TokenType::UniOperatorPlus)
	{
		parseUniOperatorStatement();
	}*/
	
	if (currentToken.Type == TokenType::Equals)
	{
		expression = "$assignment";
	}

	currentToken = getNext();

	value = currentToken.Value;
	expression = expression + getTokenValueType(currentToken);
=======
    if (Compiler::PeekNext() == nullptr)
    {
        throw std::runtime_error(&"Expected: "[type]);
    }
    
    Token currentToken = GetNext();
    
    if (currentToken.Type != type)
    {
        throw std::runtime_error(&"Expected: "[type]);
    }
>>>>>>> b9aca7a85a1442e2a2e7e3fa9fea7811306e99eb
}

/*
Check what to parse
*/
void Compiler::ParseStatement()
{
	switch (PeekNext()->Type)
	{
	case TokenType::If:
		parser.ParseIfStatement();
		break;
	case TokenType::While:
		parser.ParseLoopStatement();
		break;
	case TokenType::Identifier:
		parser.ParseAssignmentStatement();
		break;
	default:
		throw std::runtime_error("No statement found");
		break;
	}
}
