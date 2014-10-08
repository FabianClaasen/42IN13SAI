#include "InteralFunction.h"
#include "DefaultFunction.h"
#include "CompilerNode.h"
#include "Token.h"
#include <string>

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
	case TokenType::Sine:
		return mathFunction->SinFunction(identifier);
	default:
		break;
	}
    return CompilerNode("","","");
}
