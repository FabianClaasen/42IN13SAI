#include "Compiler.h"

Parser parser;

Compiler::Compiler(){}

Compiler::Compiler(std::vector<Token> tokens) : tokenizerTokens(tokens)
{
	parser = Parser(tokenizerTokens);
}

Compiler::~Compiler()
{
}

//keep parsing as long as there are tokens
void Compiler::Compile()
{
	while (currentIndex != tokenizerTokens.size()-1)
	{
		//currentSubroutine = Subroutine();
		ParseFunctionOrGlobal();
		//ParseStatement();
		currentIndex++;
	}
}


// Check what the next token is
Token* Compiler::PeekNext()
{
	Token token = tokenizerTokens.at(currentIndex+1);
    return &token;
}


// Get the next token
Token Compiler::GetNext()
{
	currentIndex++;
	Token token;
    if (currentIndex != tokenizerTokens.size()-1)
    {
		token = tokenizerTokens.at(currentIndex);
    }
    else
    {
        throw std::runtime_error("Token missing");
    }
    
    return token;
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
		parser.ParseFunction();
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
		parser.ParseAssignmentStatement();
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
		parser.ParseIfStatement();
		break;
	case TokenType::While:
		parser.ParseLoopStatement();
		break;
	case TokenType::Identifier:
		parser.ParseAssignmentStatement();
		break;
	case TokenType::Function:
		parser.ParseFunction();
		break;
	default:
		throw std::runtime_error("No statement found");
		break;
	}
}
