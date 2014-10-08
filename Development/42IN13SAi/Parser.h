#pragma once
#include "InteralFunction.h"

#include <vector>

class InteralFunction;
class Compiler;

class Parser : public Compiler
{
public:
	// Constructor
	Parser();
	virtual ~Parser();

	// Functions
	CompilerNode ParseIfStatement();
	CompilerNode ParseLoopStatement();
	CompilerNode ParseAssignmentStatement();

	CompilerNode ParseExpression();
	CompilerNode ParseRelationalExpression();
	CompilerNode ParseAddExpression();
	CompilerNode ParseMulExpression();
	CompilerNode ParseUniExpression();
	CompilerNode ParseTerm();

private:
	InteralFunction internal;
};

