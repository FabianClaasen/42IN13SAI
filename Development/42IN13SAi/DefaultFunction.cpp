#include "DefaultFunction.h"
#include "Token.h"

DefaultFunction::DefaultFunction()
{
}


DefaultFunction::~DefaultFunction()
{
}

CompilerNode DefaultFunction::Printline(Token identifier)
{
	return CompilerNode("$print", identifier.Value, identifier.Value);
}
