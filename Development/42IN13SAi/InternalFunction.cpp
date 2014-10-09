#include "InternalFunction.h"

#include "MathFunction.h"
#include "DefaultFunction.h"
#include "PhysicsFunction.h"

InternalFunction::InternalFunction()
{
	
}

InternalFunction::~InternalFunction()
{
	
}

CompilerNode InternalFunction::GetInternalFunction(Token identifier)
{
	switch (identifier.Type)
	{
	case TokenType::Cosine:
		return mathFunction->CosFunction(identifier);
	case TokenType::Sine:
		return mathFunction->SinFunction(identifier);
	default:
		break;
	}
    return CompilerNode();
}
