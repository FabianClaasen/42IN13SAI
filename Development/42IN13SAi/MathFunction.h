#pragma once
#include "InternalFunction.h"
#include "CompilerNode.h"
class MathFunction : public InternalFunction
{
public:
	MathFunction();
	virtual ~MathFunction();
	CompilerNode CosFunction();
	CompilerNode SinFunction();
	CompilerNode TanFunction();
	CompilerNode SquareFunction();
};

