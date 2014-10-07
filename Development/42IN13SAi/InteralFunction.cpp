#include "InteralFunction.h"
#include "DefaultFunction.h"
#include "CompilerNode.h"
#include "Token.h"
#include <string>

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
		return _default->CosFunction(identifier);
		break;
	case TokenType::Sine:
		_default->SinFunction;
	defaut:break;
	}
}
