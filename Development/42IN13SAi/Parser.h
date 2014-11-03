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
	virtual ~Parser();

	// Functions
	void ParseFunction();
	CompilerNode ParseAssignmentStatement();
	void ParseIfStatement();
	void ParseLoopStatement();
	void ParseFunctionCall();
    CompilerNode* ParseReturn();
    
	CompilerNode* ParseExpression();
	CompilerNode* ParseRelationalExpression();
	CompilerNode* ParseAddExpression();
	CompilerNode* ParseMulExpression();
	CompilerNode* ParseUniExpression();
	CompilerNode* ParseTerm();
	CompilerNode* ParseFunctionCall(Token token);

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

