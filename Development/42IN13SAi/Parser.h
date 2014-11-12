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
	std::shared_ptr<CompilerNode> ParseAssignmentStatement(bool forLoop);
	void ParseIfStatement();
	void ParseLoopStatement();
	void ParseFunctionCall();
    void ParseReturn();
    
	std::shared_ptr<CompilerNode> ParseExpression();
	std::shared_ptr<CompilerNode> ParseRelationalExpression();
	std::shared_ptr<CompilerNode> ParseAddExpression();
	std::shared_ptr<CompilerNode> ParseMulExpression();
	std::shared_ptr<CompilerNode> ParseUniExpression();
	std::shared_ptr<CompilerNode> ParseTerm();
	std::shared_ptr<CompilerNode> ParseFunctionCall(Token token);

private:
	Compiler* compiler;

	bool IsNextTokenUniOp();
	bool IsNextTokenMulOp();
	bool IsNextTokenAddOp();
	bool IsNextTokenRelationalOp();
	bool IsNextTokenLogicalOp();
	bool IsNextTokenReturnType();
    bool IsTokenReturnVarType(Token token);

	Symbol* GetSymbol(std::string identifier);
};

