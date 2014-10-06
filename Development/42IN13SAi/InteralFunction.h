#pragma once
#include "IInteralFunction.h"
class InteralFunction : public IInteralFunction
{
public:
	InteralFunction();
	virtual void GetInternalFunction();
	virtual ~InteralFunction();
private:
	DefaultFunction _default;
};