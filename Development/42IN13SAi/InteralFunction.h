#pragma once
#include  "IInteralFunction.h"
class DefaultFunction;
class MathFunction;
class InteralFunction : public IInteralFunction
{
public:
	InteralFunction();
	virtual void GetInternalFunction(std::string);
	virtual ~InteralFunction();
	
private:
	DefaultFunction* _default;
};
