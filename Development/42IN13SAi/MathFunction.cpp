#include "MathFunction.h"
#include "TokenType.h"
#include "Token.h"
#include "Parser.h"

MathFunction::MathFunction()
{
}


MathFunction::~MathFunction()
{

}

CompilerNode MathFunction::CosFunction(Token identifier) {
	// Logic for inside cos function

	Match(TokenType::OpenBracket);
	// Parse expression to Parser class
	CompilerNode node = parser->ParseExpression();
	Match(TokenType::CloseBracket);
	//second parameter not working yet (think has to be a token instead of string)
	return CompilerNode();
}

CompilerNode MathFunction::SinFunction(Token identifier) {
	return CompilerNode();
}
