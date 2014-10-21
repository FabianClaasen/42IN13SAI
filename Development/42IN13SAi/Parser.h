#pragma once

#include "InternalFunction.h"
#include "SymbolNotFoundException.h"
#include "Compiler.h"
#include <vector>

class Parser
{
public:
	Parser(Compiler* compiler);
	Parser(Compiler* compiler, std::vector<Token> tokens);
	virtual ~Parser() throw();

	// Functions
	void ParseFunction();
	void ParseLoopStatement();

	CompilerNode* ParseIfStatement();
	CompilerNode* ParseAssignmentStatement();
	CompilerNode* ParseExpression();
	CompilerNode* ParseRelationalExpression();
	CompilerNode* ParseAddExpression();
	CompilerNode* ParseMulExpression();
	CompilerNode* ParseUniExpression();
	CompilerNode* ParseTerm();

private:
	Compiler* compiler;

	bool IsNextTokenUniOp();
	bool IsNextTokenMulOp();
	bool IsNextTokenAddOp();
	bool IsNextTokenRelationalOp();
	bool IsNextTokenLogicalOp();
	bool IsNextTokenReturnType();

	Symbol* GetSymbol(std::string identifier);
};

