#pragma once
#include <string>
#include "Token.h"
class IInteralFunction
{
public:
	virtual ~IInteralFunction(){}
	virtual void GetInternalFunction(Token id) = 0;
};