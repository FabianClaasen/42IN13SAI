//
//  UnexpectedParameterException.cpp
//  tokenizer
//
#include "UnexpectedParameterException.h"

UnexpectedParameterException::UnexpectedParameterException(std::string m) : message(m)
{}

UnexpectedParameterException::~UnexpectedParameterException()
{}

const char* UnexpectedParameterException::what() const throw()
{
	return message.c_str();
}