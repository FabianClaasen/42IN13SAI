#pragma once
#ifndef INTERALFUNCTION_H
#define INTERAL_FUNCTION_H
#include "IInteralFunction.h"
#include "DefaultFunction.h"
class InteralFunction : public IInteralFunction
{
public:
	InteralFunction();
	virtual void GetInternalFunction(std::string);
	virtual ~InteralFunction();
private:
	DefaultFunction* _default;
};

#endif
