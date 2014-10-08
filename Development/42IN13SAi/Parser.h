#pragma once
<<<<<<< HEAD
#include "InteralFunction.h"

#include <vector>

class InteralFunction;
class Compiler;
=======
#include "Compiler.h"
>>>>>>> 24acf3a57308c73cc10a380cd43c8be74401a676

class Parser : public Compiler
{
public:
	Parser();
    virtual ~Parser() throw();

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
};

