#pragma once

#include "InternalFunction.h"
#include "SymbolNotFoundException.h"
#include <vector>

class InternalFunction;
class Compiler;

#include "Compiler.h"

class Parser : public Compiler
{
public:
	Parser();
    virtual ~Parser() throw();

	// Functions
	CompilerNode ParseIfStatement();
	void ParseLoopStatement();
	CompilerNode ParseAssignmentStatement();

	CompilerNode ParseExpression();
	CompilerNode ParseRelationalExpression();
	CompilerNode ParseAddExpression();
	CompilerNode ParseMulExpression();
	CompilerNode ParseUniExpression();
	CompilerNode ParseTerm();

private:
	bool IsNextTokenUniOp();
	bool IsNextTokenMulOp();
	bool IsNextTokenAddOp();
	bool IsNextTokenRelationalOp();
	bool IsNextTokenLogicalOp();
};

