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
	DefaultFunction* _defaultFunction;
	MathFunction*	 _mathFunction;
	PhysicsFunction* _physicsFunction;
};
