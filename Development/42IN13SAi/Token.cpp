#include <stdio.h>

#include "Token.h"

Token::Token(int lineNumber, int linePosition, int level, std::string value, TokenType tokenType, Token *partner) : LineNumber(lineNumber), LinePosition(linePosition), Level(level), Value(value), Type(tokenType), Partner(partner)
{
}

Token::Token() : LineNumber(0), LinePosition(0), Level(0), Value("")
{
}