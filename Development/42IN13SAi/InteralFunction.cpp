#include "InteralFunction.h"

#include "MathFunction.h"
#include "DefaultFunction.h"
#include "PhysicsFunction.h"

InteralFunction::InteralFunction()
{
	
}

InteralFunction::~InteralFunction()
{
	
}

CompilerNode InteralFunction::GetInternalFunction(Token identifier)
{
	switch (identifier.Type)
	{
	case TokenType::Cosine:
		return mathFunction->CosFunction(identifier);
		break;
	case TokenType::Sine:
		return mathFunction->SinFunction(identifier);
        break;
	default:
		break;
	}
    return CompilerNode();
}
