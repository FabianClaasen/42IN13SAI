#pragma once

#include <list>
#include <iterator>
#include <string>
#include <memory>
#include <iostream>

#include "CompilerNode.h"
#include "Token.h"
#include "Symbol.h"
#include "SymbolTable.h"
#include "Subroutine.h"
#include "SubroutineTable.h"

#include "MissingTokenException.h"
#include "StatementNotFoundException.h"
#include "VariableNotFoundException.h"
#include "UnexpectedTypeException.h"
#include "UnexpectedKeywordException.h"
#include "ParameterNameException.h"
#include "IdentifierException.h"
#include "UnexpectedParameterException.h"

class InternalFunction;
class Parser;
class Compiler
{
public:
	Compiler(std::vector<Token> tokens);
	virtual ~Compiler();

	// Functions
	void Compile();
	Token* PeekNext();
	Token  GetNext();
	Subroutine* GetSubroutine();
	Symbol* GetSymbol(std::string name);
	void SetTokenList(std::vector<Token> tokens);
	void SetSubroutine(Subroutine subroutine);
	void AddSubroutine();
	void AddSymbol(Symbol symbol);
	void AddCompilerNode(std::shared_ptr<CompilerNode> node);
	bool HasSymbol(std::string symbolName);
	void ParseGlobalStatement();
	void ParseStatement();
	bool IsInternalFunction(MyTokenType type);
	CompilerNode ParseInternalFunction();

	void Match(MyTokenType type);

	//Info to send to VM
	SymbolTable* GetSymbolTable();
	SubroutineTable* GetSubroutineTable();
	std::list<std::shared_ptr<CompilerNode>> GetCompilerNodes();
	
private:
	// Variables
	//std::shared_ptr<InternalFunction> internalFunction;
	//std::shared_ptr<Parser> parser;
	std::vector<Token> tokenizerTokens;
	std::list<std::shared_ptr<CompilerNode>> compilerNodes;
	SymbolTable symbolTable;
	SubroutineTable subroutineTable;
	Subroutine currentSubroutine;
	int currentToken = 0;
	int currentIndex = -1;
	void ParseFunctionOrGlobal();
	void ParseFunctionOrAssignment();
	std::string TokenToString(MyTokenType type);
};