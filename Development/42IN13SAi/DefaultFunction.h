#pragma once
#include "InternalFunction.h"

class DefaultFunction : public InternalFunction
{
public:
	DefaultFunction();
	virtual ~DefaultFunction();

	CompilerNode Printline();
	CompilerNode Stop();
};
