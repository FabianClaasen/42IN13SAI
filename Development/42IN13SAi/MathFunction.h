#pragma once
#include "InteralFunction.h"
#include "CompilerNode.h"
class MathFunction : public InteralFunction
{
public:
	MathFunction();
	virtual ~MathFunction();
	CompilerNode CosFunction(Token id);
	CompilerNode SinFunction(Token id);
};

