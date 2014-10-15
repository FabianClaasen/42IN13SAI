#include "Compiler.h"
#include "Parser.h"

Compiler::Compiler()
{

}

Compiler::Compiler(std::list<Token> tokens) : tokenizerTokens(tokens)
{
	Compile();
}

Compiler::~Compiler()
{
}


//keep parsing as long as there are tokens
void Compiler::Compile()
{
	while (Compiler::PeekNext() != nullptr)
	{
		currentSubroutine = Subroutine();
		ParseFunctionOrGlobal();
		//ParseStatement();
	}
}


// Check what the next token is
Token* Compiler::PeekNext()
{
    std::list<Token>::iterator it = tokenizerTokens.begin();
    std::advance(it, currentToken + 1);
    
    return &*it;
}


// Get the next token
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
    if (Compiler::PeekNext() == nullptr)
    {
        throw std::runtime_error(&"Expected: "[int(type)]);
    }
    
    Token currentToken = GetNext(); // Bestaat al
    
    if (currentToken.Type != type)
    {
        throw std::runtime_error(&"Expected: "[int(type)]);
    }
}

// Check if you need to parse a function or a global.
// This is needed to save the symbols and compiler nodes in the right file.
void Compiler::ParseFunctionOrGlobal()
{
	switch (PeekNext()->Type)
	{
	case TokenType::Function:
		parser->ParseFunction();
		break;
	default:
		ParseGlobalStatement();
		break;
	}
}

// Check if the token type starts with a Var token.
// A global can only be a variable thats why it can only be an assignment.
void Compiler::ParseGlobalStatement()
{
	switch (PeekNext()->Type)
	{
	case TokenType::Var:
		parser->ParseAssignmentStatement();
		break;
	default:
		throw std::runtime_error("No variable found");
		break;
	}
}

// Parse statements.
void Compiler::ParseStatement()
{
	switch (PeekNext()->Type)
	{
	case TokenType::If:
		parser->ParseIfStatement();
		break;
	case TokenType::While:
		parser->ParseLoopStatement();
		break;
	case TokenType::Identifier:
		parser->ParseAssignmentStatement();
		break;
	case TokenType::Function:
		parser->ParseFunction();
		break;
	default:
		throw std::runtime_error("No statement found");
		break;
	}
}
