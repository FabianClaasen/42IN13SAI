#pragma once
#include <list>
#include <iterator>
#include <string>
#include "CompilerNode.h"
#include "Token.h"
#include "Symbol.h"
#include "SymbolTable.h"
#include "Subroutine.h"
#include "SubroutineTable.h"

class Compiler
{
public:
	Compiler();
	Compiler(std::list<Token> tokens);
	virtual ~Compiler() throw();
	
private:
	std::list<Token>::iterator it;

protected:
	// Variables
	
	std::list<Token> tokenizerTokens;
	std::list<CompilerNode> *compilerNodes;
	SymbolTable symbolTable;
	SubroutineTable subroutineTable;

	// Functions
	Token*  PeekNext();
	Token  GetNext();
	void Match(TokenType type);
	void ParseStatement();

private:
	int currentToken = 0;
	void Compile();
};