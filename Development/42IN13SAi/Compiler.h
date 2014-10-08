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

<<<<<<< HEAD
private:
	const std::list<Token>::iterator it;
=======
protected:
	// Variables
	Parser parser;
>>>>>>> b9aca7a85a1442e2a2e7e3fa9fea7811306e99eb
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