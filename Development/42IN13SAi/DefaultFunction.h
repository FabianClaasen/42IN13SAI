#pragma once
#include "InternalFunction.h"

class DefaultFunction : public InternalFunction
{
public:
	DefaultFunction();
	virtual ~DefaultFunction();
private:
	CompilerNode Printline();
	CompilerNode Stop();
};
