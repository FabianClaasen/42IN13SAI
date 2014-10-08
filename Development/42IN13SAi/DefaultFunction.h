#pragma once
#include "InteralFunction.h";
#include "CompilerNode.h"
#include "Token.h"
class DefaultFunction : public InteralFunction
{
public:
	DefaultFunction();
	virtual ~DefaultFunction();
private:
	CompilerNode Printline(Token identifier);
	CompilerNode Stop(Token identifier);
};
