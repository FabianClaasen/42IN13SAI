#pragma once
#include  "IInteralFunction.h"
class DefaultFunction;
class InteralFunction : public IInteralFunction
{
public:
	InteralFunction(DefaultFunction &);
	virtual void GetInternalFunction(std::string);
	virtual ~InteralFunction();
	
private:
	
};
