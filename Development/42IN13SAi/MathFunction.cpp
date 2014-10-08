#include "MathFunction.h"

MathFunction::MathFunction()
{
}


MathFunction::~MathFunction()
{

}

CompilerNode MathFunction::CosFunction(Token identifier) {
	//PeekNext();
	//second parameter not working yet (think has to be a token instead of string)
	return CompilerNode(std::string("$cos"), identifier.Value, identifier.Value);
}

CompilerNode MathFunction::SinFunction(Token identifier) {
	return CompilerNode(std::string("$sin"), identifier.Value, identifier.Value);
}
