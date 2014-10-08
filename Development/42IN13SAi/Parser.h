#pragma once
#include "InteralFunction.h"

class InteralFunction;
class Compiler;

class Parser : public Compiler
{
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

private:
	InteralFunction internal;
};

