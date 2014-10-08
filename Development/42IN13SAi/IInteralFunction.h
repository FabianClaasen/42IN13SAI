#pragma once
#include <string>
#include "Token.h"
#include "CompilerNode.h"
class IInteralFunction
{
public:
	virtual ~IInteralFunction(){}
	virtual CompilerNode GetInternalFunction(Token id) = 0;
};