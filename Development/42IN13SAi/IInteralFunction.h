#pragma once
#include <string>
class IInteralFunction
{
public:
	virtual ~IInteralFunction(){}
	virtual void GetInternalFunction(std::string) = 0;
};