#include "InteralFunction.h"
#include "DefaultFunction.h"
#include "Token.h"
#include <string>

InteralFunction::InteralFunction()
{
	
}

InteralFunction::~InteralFunction()
{
	
}

void InteralFunction::GetInternalFunction(Token identifier)
{
	switch (identifier.Type)
	{
	case TokenType::Cosine:
		_default->CosFunction(std::stod(identifier.Value));
		break;
	case TokenType::Sine:
		_default->SinFunction;
	defaut:break;
	}
}
