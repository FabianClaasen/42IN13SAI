//
//  VariableNotFoundException.cpp
//  tokenizer
//
#include "VariableNotFoundException.h"

VariableNotFoundException::VariableNotFoundException(std::string m) : message(m)
{}

VariableNotFoundException::~VariableNotFoundException()
{}

const char* VariableNotFoundException::what() const throw()
{
	return message.c_str();
}