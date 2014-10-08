#include "DefaultFunction.h"
DefaultFunction::DefaultFunction()
{
}


DefaultFunction::~DefaultFunction()
{
}

CompilerNode DefaultFunction::CosFunction(Token identifier) {
	//second parameter not working yet (think has to be a token instead of string)
	return CompilerNode(std::string("$cos"), identifier.Value, identifier.Value);
}

CompilerNode DefaultFunction::SinFunction(Token identifier) {
	return CompilerNode();
}
