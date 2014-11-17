//
//  ParameterNameException.cpp
//  tokenizer
//
#include "ParameterNameException.h"

ParameterNameException::ParameterNameException(std::string m) : message(m)
{}

ParameterNameException::~ParameterNameException()
{}

const char* ParameterNameException::what() const throw()
{
	return message.c_str();
}