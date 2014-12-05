#include <stdio.h>
#include "Token.h"

Token::Token(int lineNumber, int linePosition, int level, std::string value, MyTokenType myTokenType, std::shared_ptr<Token> partner) : LineNumber(lineNumber), LinePosition(linePosition), Level(level), Value(value), Type(myTokenType)
{
	Partner = partner;
}

Token::Token(const Token & other): LineNumber(other.LineNumber), LinePosition(other.LinePosition), Level(other.Level), Value(other.Value), Type(other.Type), Partner(other.Partner)
{
    
}

Token::Token() : LineNumber(0), LinePosition(0), Level(0), Value(""), Partner(std::shared_ptr<Token>())
{
}

Token::~Token()
{
	
}