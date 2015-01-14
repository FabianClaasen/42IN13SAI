#pragma once

#include <list>
#include <iterator>
#include <string>
#include <memory>
#include <iostream>

#include "CompilerNode.h"
#include "Token.h"
#include "Symbol.h"
#include "SymbolTable.h"
#include "Subroutine.h"
#include "SubroutineTable.h"

#include "MissingTokenException.h"
#include "StatementNotFoundException.h"
#include "VariableNotFoundException.h"
#include "UnexpectedTypeException.h"
#include "UnexpectedKeywordException.h"
#include "ParameterNameException.h"
#include "IdentifierException.h"
#include "UnexpectedParameterException.h"
#include "DiagnosticsBuilder.h"

class InternalFunction;
class Parser;
class Compiler
{
public:
	Compiler(std::vector<std::shared_ptr<Token>> tokens);
	virtual ~Compiler();

	// Functions
	void Compile();
    
	std::shared_ptr<Token> PeekNext();
	Token  GetNext();
    Token GetCurrent();
    void SkipUntil(MyTokenType tokenType);
    void SkipUntil(Token token);
	
    void SetTokenList(std::vector<std::shared_ptr<Token>> tokens);
    
    Subroutine* GetSubroutine();
	void SetSubroutine(Subroutine subroutine);
	void AddSubroutine();
	
    void AddCompilerNode(std::shared_ptr<CompilerNode> node);
    
	bool HasSymbol(std::string symbolName);
    Symbol* GetSymbol(std::string name);
    void AddSymbol(Symbol symbol);
    
	void ParseGlobalStatement();
	void ParseStatement();
	bool IsInternalFunction(MyTokenType type);
	bool IsConstant(MyTokenType type);
	CompilerNode ParseInternalFunction();
	CompilerNode ParseConstant();

	void Match(MyTokenType type);
	bool HasExceptions();

	//Info to send to VM
	SymbolTable* GetSymbolTable();
	SubroutineTable* GetSubroutineTable();
	std::list<std::shared_ptr<CompilerNode>> GetCompilerNodes();
    
    /// Returns a DiagnosticBuilder for the exception occuring
    ///
    /// @see ExceptionEnum.h
    /// @param token The Token the exception happened on
    /// @param exception The exception that happened
    ///
    DiagnosticBuilder Diag(ExceptionEnum exception);
	
private:
	// Variables
	//std::shared_ptr<InternalFunction> internalFunction;
	//std::shared_ptr<Parser> parser;
	std::vector<std::shared_ptr<Token>> tokenizerTokens;
	std::list<std::shared_ptr<CompilerNode>> compilerNodes;
	SymbolTable symbolTable;
	SubroutineTable subroutineTable;
	Subroutine currentSubroutine;
	int currentToken = 0;
	int currentIndex = -1;
	void ParseFunctionOrGlobal();
	void ParseFunctionOrAssignment();
	std::string TokenToString(MyTokenType type);
    
    
    bool hasExceptions;
    bool isFatalException;
};
