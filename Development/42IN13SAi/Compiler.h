#pragma once

#include <list>
#include <iterator>
#include <string>
#include <memory>

#include "CompilerNode.h"
#include "Token.h"
#include "Symbol.h"
#include "SymbolTable.h"
#include "Subroutine.h"
#include "SubroutineTable.h"

class InternalFunction;
class Parser;
class Compiler
{
public:
	Compiler(std::vector<Token> tokens);
	virtual ~Compiler() throw();

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
	bool HasSymbol(std::string symbolName);
	void ParseGlobalStatement();
	void ParseStatement();
	void Match(TokenType type);

	//Info to send to VM
	SymbolTable GetSymbolTable();
	SubroutineTable GetSubroutineTable();
	std::vector<CompilerNode> GetCompilerNodes();
	
private:
	// Variables
	//std::shared_ptr<InternalFunction> internalFunction;
	//std::shared_ptr<Parser> parser;
	std::vector<Token> tokenizerTokens;
	std::vector<CompilerNode> compilerNodes;
	SymbolTable symbolTable;
	SubroutineTable subroutineTable;
	Subroutine currentSubroutine;
	int currentToken = 0;
	int currentIndex = -1;
	void ParseFunctionOrGlobal();
};