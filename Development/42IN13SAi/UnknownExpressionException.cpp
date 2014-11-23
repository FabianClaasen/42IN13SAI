//
//  UnknownExpressionException.cpp
//  Virtual Machine
//

#include "UnknownExpressionException.h"

UnknownExpressionException::UnknownExpressionException(std::string m) : message(m)
{}

UnknownExpressionException::~UnknownExpressionException()
{}

const char* UnknownExpressionException::what() const throw()
{
	return message.c_str();
}