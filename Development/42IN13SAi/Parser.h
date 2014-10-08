#pragma once
#include "Compiler.h"
#include "InteralFunction.h"

class Parser :
	public Compiler
{
private:
	InteralFunction internal;

public:
	// Constructor
	Parser();
	virtual ~Parser();

	// Functions
	void ParseIfStatement();
	void ParseLoopStatement();
	void ParseAssignmentStatement();

	CompilerNode ParseExpression();
	CompilerNode ParseRelationalExpression();
	CompilerNode ParseAddExpression();
	CompilerNode ParseMulExpression();
	CompilerNode ParseUniExpression();
	CompilerNode ParseTerm();

	std::string GetTokenValueType(Token currentToken);
};

