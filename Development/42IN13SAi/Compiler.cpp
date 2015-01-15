#include "Compiler.h"
#include "InternalFunction.h"
#include "Constant.h"
#include "Parser.h"

Compiler::Compiler(std::vector<std::shared_ptr<Token>> tokens) : tokenizerTokens(tokens)
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
		ParseFunctionOrGlobal();
	}
}


// Check what the next token is
std::shared_ptr<Token> Compiler::PeekNext()
{
	std::shared_ptr<Token> token = tokenizerTokens.at(currentIndex + 1);
	return token;
}


// Get the next token
Token Compiler::GetNext()
{
	currentIndex++;
	Token token;
	if (currentIndex != tokenizerTokens.size())
	{
		token = *tokenizerTokens.at(currentIndex);
	}
	else
	{
        throw MissingTokenException("A MissingTokenException occured.");
	}

	return token;
}

Token Compiler::GetCurrent()
{
    return *tokenizerTokens.at(currentIndex);
}

void Compiler::SkipUntil(MyTokenType tokenType)
{
    int level = tokenizerTokens.at(currentIndex)->Level;
    
    while (tokenizerTokens.at(currentIndex)->Type != tokenType || tokenizerTokens.at(currentIndex)->Level != level)
    {
        currentIndex++;
    }
    
    if (tokenizerTokens.at(currentIndex)->Type == MyTokenType::EOL)
        currentIndex++;
}

void Compiler::SkipUntil(Token token)
{
    Token toSkipTo = *tokenizerTokens.at(currentIndex);
    while (&toSkipTo != &token)
    {
        currentIndex++;
    }
}

// Set the tokenlist
void Compiler::SetTokenList(std::vector<std::shared_ptr<Token>> tokens)
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
void Compiler::AddCompilerNode(std::shared_ptr<CompilerNode> node)
{
	std::shared_ptr<CompilerNode> sNode = std::shared_ptr<CompilerNode>(node);
	compilerNodes.push_back(sNode);
}

bool Compiler::HasSymbol(std::string symbolName)
{
	return symbolTable.HasSymbol(symbolName);
}

// Match the next token with the given type
void Compiler::Match(MyTokenType type)
{
	if (Compiler::PeekNext() == nullptr)
	{
		//exceptions.push_back("An UnexpectedTypeException occured at line " + std::to_string(PeekNext()->LineNumber) + " on position " + std::to_string(PeekNext()->LinePosition) + ". Expected: " + TokenToString(type));
		throw UnexpectedTypeException("An UnexpectedTypeException occured at line " + std::to_string(PeekNext()->LineNumber) + " on position " + std::to_string(PeekNext()->LinePosition) + ". Expected: " + TokenToString(type));
	}

	Token currentToken = GetNext(); // Bestaat al

	if (currentToken.Type != type)
	{
        //exceptions.push_back("An UnexpectedTypeException occured at line " + std::to_string(PeekNext()->LineNumber) + " on position " + std::to_string(PeekNext()->LinePosition) + ". Expected: " + TokenToString(type));
		throw UnexpectedTypeException("An UnexpectedTypeException occured at line " + std::to_string(currentToken.LineNumber) + " on position " + std::to_string(currentToken.LinePosition) + ". Expected: " + TokenToString(type));
	}
}

// Check if you need to parse a function or a global.
// This is needed to save the symbols and compiler nodes in the right file.
void Compiler::ParseFunctionOrGlobal()
{
	switch (PeekNext()->Type)
	{
	case MyTokenType::Function:
	case MyTokenType::MainFunction:
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
	case MyTokenType::Var:
		Parser(this).ParseAssignmentStatement(false);
		break;
	default:
        //exceptions.push_back("A VariableNotFoundException occured at line " + std::to_string(PeekNext()->LineNumber) + " on position " + std::to_string(PeekNext()->LinePosition) + ".");
        throw VariableNotFoundException("A VariableNotFoundException occured at line " + std::to_string(PeekNext()->LineNumber) + " on position " + std::to_string(PeekNext()->LinePosition) + ".");
		break;
	}
}

// Parse statements.
void Compiler::ParseStatement()
{
	switch (PeekNext()->Type)
	{
    case MyTokenType::ElseIf:
	case MyTokenType::If:
		Parser(this).ParseIfStatement();
		break;
	case MyTokenType::While: case MyTokenType::ForLoop:
		Parser(this).ParseLoopStatement();
		break;
	case MyTokenType::Identifier:
		ParseFunctionOrAssignment();
		break;
	case MyTokenType::Var:
		Parser(this).ParseAssignmentStatement(false);
		break;
	case MyTokenType::MainFunction:
	case MyTokenType::Function:
		Parser(this).ParseFunction();
		break;
	case MyTokenType::Return:	
		Parser(this).ParseReturn();
		break;
	case MyTokenType::PrintLine:
		if (!currentSubroutine.isEmpty)
			currentSubroutine.AddCompilerNode(std::shared_ptr<CompilerNode>(InternalFunction(this).GetInternalFunction(MyTokenType::PrintLine)));
		Match(MyTokenType::EOL);
		break;
	case MyTokenType::Stop:
		if (!currentSubroutine.isEmpty)
			currentSubroutine.AddCompilerNode(std::shared_ptr<CompilerNode>(InternalFunction(this).GetInternalFunction(MyTokenType::Stop)));
			Match(MyTokenType::EOL);
		break;
	case MyTokenType::AbcFormula:
		if (!currentSubroutine.isEmpty)
			currentSubroutine.AddCompilerNode(std::shared_ptr<CompilerNode>(InternalFunction(this).GetInternalFunction(MyTokenType::AbcFormula)));
		Match(MyTokenType::EOL);
		break;
	default:
            Diag(ExceptionEnum::err_var_not_found) << PeekNext()->Value << PeekNext()->LineNumber;
            SkipUntil(MyTokenType::EOL);
		break;
	}
}

void Compiler::ParseFunctionOrAssignment()
{
	std::shared_ptr<Token> temp = std::shared_ptr<Token>(new Token(GetNext()));
	if (PeekNext()->Type == MyTokenType::OpenBracket)
	{
		tokenizerTokens.insert(tokenizerTokens.begin(), temp);
		Parser(this).ParseFunctionCall();
	}
	else
	{
		tokenizerTokens.insert(tokenizerTokens.begin(), temp);
		Parser(this).ParseAssignmentStatement(false);
	}
}

// Internal functions
bool Compiler::IsInternalFunction(MyTokenType type)
{
	//check if type is an internal function
	switch (type)
	{
		// Default functions
		case MyTokenType::Stop:
		case MyTokenType::PrintLine:
		// Math functions
		case MyTokenType::Cosine:
		case MyTokenType::Sine:
		case MyTokenType::Tangent:
		case MyTokenType::Square:
		case MyTokenType::Cube:
		case MyTokenType::Power:
		case MyTokenType::SquareRoot:
		case MyTokenType::CubeRoot:
		case MyTokenType::Degree:
		case MyTokenType::Radiant:
		case MyTokenType::Percent:
		case MyTokenType::Permillage:
		case MyTokenType::CommonLog:
		case MyTokenType::BinaryLog:
		case MyTokenType::NaturalLog:
		case MyTokenType::Log:
		case MyTokenType::Modulo:
		case MyTokenType::Gcd:
		case MyTokenType::Fibonacci:
		case MyTokenType::Pythagoras:
		case MyTokenType::PythagorasH:
		case MyTokenType::Distance:
		case MyTokenType::Velocity:
		case MyTokenType::Time:
		case MyTokenType::AverageVelocity:
		case MyTokenType::Acceleration:
		case MyTokenType::Discriminant:
		case MyTokenType::AbcFormula:
			return true;
		default:
			return false;
	}
}

// Internal functions
bool Compiler::IsConstant(MyTokenType type)
{
	//check if type is an internal function
	switch (type)
	{
		// Default functions
	case MyTokenType::Math_E:
	case MyTokenType::Math_PI:
		return true;
	default:
		return false;
	}
}


CompilerNode Compiler::ParseInternalFunction()
{
	return *InternalFunction(this).GetInternalFunction(PeekNext()->Type);
}

CompilerNode Compiler::ParseConstant()
{
	return *Constant(this).GetConstant(PeekNext()->Type);
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
std::list<std::shared_ptr<CompilerNode>> Compiler::GetCompilerNodes()
{
	return compilerNodes;
}


std::string Compiler::TokenToString(MyTokenType type)
{
	switch (type)
	{
	case MyTokenType::OpenBracket:
		return "(";
	case MyTokenType::CloseBracket:
		return ")";
	case MyTokenType::OpenCurlyBracket:
		return "{";
	case MyTokenType::CloseCurlyBracket:
		return "}";
	case MyTokenType::OpenMethod:
		return "[";
	case MyTokenType::CloseMethod:
		return "]";
	case MyTokenType::EOL:
		return ";";
	case MyTokenType::Separator:
		return "|";
	default:
		return "undefined";
	}
}

DiagnosticBuilder Compiler::Diag(ExceptionEnum exception)
{
    hasExceptions = true;
    return DiagnosticBuilder(exception);
}

bool Compiler::HasExceptions()
{
	return hasExceptions;
}
