#pragma once
#include "Token.h"
#include <list>
#include "CompilerNode.h"
class Compiler
{
public:
	Compiler(std::list<Token> tokens);
	~Compiler();

private:
	std::list<Token> tokenizerTokens;
	std::list<CompilerNode> *compilerNodes;
	int currentToken = 0;

	Token* peekNext();
	Token getNext();
	void match(TokenType type);
	void parseExpression();
	void parseIfStatement();
	void parseLoopStatement();
	void parseAssignmentStatement();
	std::string getTokenValueType(Token currentToken);
	void parseStatement();
	void compile();
};