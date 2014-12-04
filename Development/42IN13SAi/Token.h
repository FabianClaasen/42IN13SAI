#pragma once
#include <memory>
#include <string>
#include "MyTokenType.h"


class Token
{
public:
	int LineNumber;
	int LinePosition;
	int Level;
	std::string Value;
	MyTokenType Type;
	std::shared_ptr<Token> Partner;

	Token(int lineNumber, int linePosition, int level, std::string value, MyTokenType myTokenType, std::shared_ptr<Token> partner);
    Token(const Token &);
	Token();
	~Token();
};