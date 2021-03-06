#include <stdio.h>

#include "Token.h"

Token::Token(int lineNumber, int linePosition, int level, std::string value, MyTokenType myTokenType, Token* partner) : LineNumber(lineNumber), LinePosition(linePosition), Level(level), Value(value), Type(myTokenType), Partner(partner)
{
}

Token::Token() : LineNumber(0), LinePosition(0), Level(0), Value(""), Partner(nullptr)
{
}

Token::~Token()
{
	try{
		delete Partner;
	}
	catch (...)
	{
		throw;
	}
}