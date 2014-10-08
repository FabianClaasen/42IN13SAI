#pragma once
#include "Token.h"
#include <list>
#include "CompilerNode.h"
#include "Parser.h"

class Compiler
{
public:
	Compiler();
	Compiler(std::list<Token> tokens);
	virtual ~Compiler();

private:
	const std::list<Token>::iterator it;

protected:
	// Variables
	Parser parser;
	std::list<Token> tokenizerTokens;
	std::list<CompilerNode> *compilerNodes;

	// Functions
	Token* PeekNext();
	Token GetNext();
	void Match(TokenType type);
	void ParseStatement();

private:
	int currentToken = 0;
	void Compile();
};