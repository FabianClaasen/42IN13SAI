//
//  IdentifierException.cpp
//  tokenizer
//
#include "IdentifierException.h"

IdentifierException::IdentifierException(std::string m) : message(m)
{}

IdentifierException::~IdentifierException()
{}

const char* IdentifierException::what() const throw()
{
	return message.c_str();
}