#pragma once
#include  "IInteralFunction.h"
#include "Token.h"
#include "CompilerNode.h"
class DefaultFunction;
class MathFunction;
class InteralFunction : public IInteralFunction
{
public:
	InteralFunction();
	virtual CompilerNode GetInternalFunction(Token id);
	virtual ~InteralFunction();
	
private:
	DefaultFunction* _default;
};
