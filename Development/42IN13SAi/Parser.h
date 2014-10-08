#pragma once
#include "Compiler.h"

class Parser : public Compiler
{
public:
	Parser();
    virtual ~Parser() throw();

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

private:
};

