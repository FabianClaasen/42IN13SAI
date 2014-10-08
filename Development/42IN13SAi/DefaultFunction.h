#pragma once
#include "InteralFunction.h"

class DefaultFunction : public InteralFunction
{
public:
	DefaultFunction();
	virtual ~DefaultFunction();
private:
	CompilerNode Printline(Token identifier);
	CompilerNode Stop(Token identifier);
};
