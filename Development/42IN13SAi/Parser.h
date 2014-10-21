#pragma once

#include "InternalFunction.h"
#include "SymbolNotFoundException.h"
#include "Compiler.h"
#include <vector>

class Parser : public Compiler
{
public:
	Parser();
	Parser(std::vector<Token> tokens);
	Parser& operator=(const Parser& other);
    virtual ~Parser() throw();

	// Functions
	void ParseFunction();
	void ParseLoopStatement();

	CompilerNode ParseIfStatement();
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
	bool IsNextTokenReturnType();

	Symbol* GetSymbol(std::string identifier);
};

