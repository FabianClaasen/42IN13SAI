#pragma once
#include "InternalFunction.h"

class DefaultFunction : public InternalFunction
{
public:
	DefaultFunction();
	virtual ~DefaultFunction();
private:
	CompilerNode Printline(Token identifier);
	CompilerNode Stop(Token identifier);
};
