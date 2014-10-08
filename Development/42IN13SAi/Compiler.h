#pragma once
#include "Token.h"
#include "Parser.h"
#include <list>
#include "CompilerNode.h"

class Compiler
{
public:
	Compiler();
	Compiler(std::list<Token> tokens);
	~Compiler();

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