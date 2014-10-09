#pragma once
#include "InternalFunction.h"
#include "CompilerNode.h"
class MathFunction : public InternalFunction
{
public:
	MathFunction();
	virtual ~MathFunction();
	CompilerNode CosFunction(Token id);
	CompilerNode SinFunction(Token id);
};

