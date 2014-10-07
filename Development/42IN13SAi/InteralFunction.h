#pragma once
#include  "IInteralFunction.h"
#include "Token.h"
class DefaultFunction;
class MathFunction;
class InteralFunction : public IInteralFunction
{
public:
	InteralFunction();
	virtual void GetInternalFunction(Token id);
	virtual ~InteralFunction();
	
private:
	DefaultFunction* _default;
};
