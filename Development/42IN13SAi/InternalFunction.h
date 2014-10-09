#pragma once
#include <string>
#include "Compiler.h"

class DefaultFunction;
class MathFunction;
class PhysicsFunction;

class InternalFunction : public Compiler 
{
public:
	InternalFunction();
	CompilerNode GetInternalFunction(Token id);
	virtual ~InternalFunction();
	
private:
	DefaultFunction* defaultFunction;
	MathFunction*	 mathFunction;
	PhysicsFunction* physicsFunction;
};
