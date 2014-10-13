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
		ParseStatement();
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


// Check what to parse
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
