#pragma once
#include "Token.h"
#include "CompilerNode.h"
#include "Compiler.h"
class DefaultFunction;
class MathFunction;
class PhysicsFunction;

class InteralFunction : public Compiler 
{
public:
	InteralFunction();
	CompilerNode GetInternalFunction(Token id);
	virtual ~InteralFunction();
	
private:
	DefaultFunction* defaultFunction;
	MathFunction*	 mathFunction;
	PhysicsFunction* physicsFunction;
};
