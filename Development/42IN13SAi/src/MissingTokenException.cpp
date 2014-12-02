//
//  MissingTokenException.cpp
//  tokenizer
//
#include "MissingTokenException.h"

MissingTokenException::MissingTokenException(std::string m) : message(m)
{}

MissingTokenException::~MissingTokenException()
{}

const char* MissingTokenException::what() const throw()
{
	return message.c_str();
}
