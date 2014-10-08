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
		break;
	case TokenType::Sine:
		return mathFunction->SinFunction(identifier);
        break;
	default:
		break;
	}
    return CompilerNode("","","");
}
