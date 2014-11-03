#include "Compiler.h"
#include "InternalFunction.h"
#include "Parser.h"

Compiler::Compiler(std::vector<Token> tokens) : tokenizerTokens(tokens)
{
}

Compiler::~Compiler()
{
}

//keep parsing as long as there are tokens
void Compiler::Compile()
{
	while (currentIndex != tokenizerTokens.size() - 1)
	{
		currentSubroutine = Subroutine();
		ParseStatement();
	}
}


// Check what the next token is
Token* Compiler::PeekNext()
{
    Token* token = &tokenizerTokens.at(currentIndex + 1);
	return token;
}


// Get the next token
Token Compiler::GetNext()
{
	currentIndex++;
	Token token;
	if (currentIndex != tokenizerTokens.size())
	{
		token = tokenizerTokens.at(currentIndex);
	}
	else
	{
		throw std::runtime_error("Token missing");
	}

	return token;
}

// Set the tokenlist
void Compiler::SetTokenList(std::vector<Token> tokens)
{
	tokenizerTokens = tokens;
}

// Get the currentSubroutine
Subroutine* Compiler::GetSubroutine()
{
	return &currentSubroutine;
}

// Get a symbol
Symbol* Compiler::GetSymbol(std::string name)
{
	return symbolTable.GetSymbol(name);
}

// Set the currentSubroutine
void Compiler::SetSubroutine(Subroutine subroutine)
{
	currentSubroutine = subroutine;
}

// Add a subroutine
void Compiler::AddSubroutine()
{
	subroutineTable.AddSubroutine(currentSubroutine);
}

// Add a symbol
void Compiler::AddSymbol(Symbol symbol)
{
	symbolTable.AddSymbol(symbol);
}

// Add a compiler node
void Compiler::AddCompilerNode(CompilerNode node)
{
	compilerNodes.push_back(node);
}

bool Compiler::HasSymbol(std::string symbolName)
{
	return symbolTable.HasSymbol(symbolName);
}

// Match the next token with the given type
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
		Parser(this).ParseFunction();
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
		Parser(this).ParseAssignmentStatement();
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
		Parser(this).ParseIfStatement();
		break;
	case TokenType::While:
		Parser(this).ParseLoopStatement();
		break;
	case TokenType::Identifier:
		ParseFunctionOrAssignment();
		break;
	case TokenType::Var:
		Parser(this).ParseAssignmentStatement();
		break;
    case TokenType::MainFunction:
	case TokenType::Function:
		Parser(this).ParseFunction();
		break;
    case TokenType::Return:
        Parser(this).ParseReturn();
        break;
	case TokenType::PrintLine:
		if (!currentSubroutine.isEmpty)
			currentSubroutine.AddCompilerNode(InternalFunction(this).GetInternalFunction(TokenType::PrintLine));
            Match(TokenType::EOL);
		//else
			//std::runtime_error("");
		break;
	case TokenType::Stop:
		if (!currentSubroutine.isEmpty)
			currentSubroutine.AddCompilerNode(InternalFunction(this).GetInternalFunction(TokenType::Stop));
            Match(TokenType::EOL);
		//else
		//std::runtime_error("");
		break;
	default:
		throw std::runtime_error("No statement found");
		break;
	}
}

void Compiler::ParseFunctionOrAssignment()
{
	Token temp = GetNext();
	if (PeekNext()->Type == TokenType::OpenBracket)
	{
		tokenizerTokens.insert(tokenizerTokens.begin(), temp);
		Parser(this).ParseFunctionCall();
	}
	else
	{
		tokenizerTokens.insert(tokenizerTokens.begin(), temp);
		Parser(this).ParseAssignmentStatement();
	}
}

// Internal functions
bool Compiler::IsInternalFunction(TokenType type)
{
	//check if type is an internal function
	switch (type)
	{
		// Default functions
		case TokenType::Stop:
		case TokenType::PrintLine:
		// Math functions
		case TokenType::Cosine:
		case TokenType::Sine:
		case TokenType::Tangent:
		case TokenType::Square:
		case TokenType::Power:
		case TokenType::SquareRoot:
		case TokenType::CubeRoot:
		case TokenType::Degree:
		case TokenType::Radiant:
		case TokenType::Percent:
		case TokenType::PerMillage:
		case TokenType::NormalLog:
		case TokenType::NormaltwoLog:
		case TokenType::Nlog:
		case TokenType::Modulo:
			return true;
		default:
			return false;
	}
}

CompilerNode Compiler::ParseInternalFunction()
{
	return InternalFunction(this).GetInternalFunction(PeekNext()->Type);
}



//Get symbol table for the VM
SymbolTable* Compiler::GetSymbolTable()
{
	return &symbolTable;
}

//Get Subroutine table for the VM
SubroutineTable* Compiler::GetSubroutineTable()
{
	return &subroutineTable;
}

// Get the Compiler nodes (list) for the VM
std::vector<CompilerNode> Compiler::GetCompilerNodes()
{
	return compilerNodes;
}
