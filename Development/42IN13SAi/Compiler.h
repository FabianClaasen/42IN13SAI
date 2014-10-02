#pragma once
class Compiler
{
public:
	Compiler(std::list<Token> tokens);
	~Compiler();

private:
	std::list<Token> tokenizerTokens;
	std::list<CompilerNode> *compilerNodes;
	int currentToken = 0;

	Token peekNext();
	Token getNext();
	void parseIfStatement();
	void parseWhileStatement();
	void parseAssignmentStatement();
	void parseStatement();
	void compile();
};