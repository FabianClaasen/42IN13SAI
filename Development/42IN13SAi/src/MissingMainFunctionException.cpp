//
//  MissingMainFunctionException.cpp
//  Virtual Machine
//

#include "MissingMainFunctionException.h"

MissingMainFunctionException::MissingMainFunctionException(std::string m) : message(m)
{}

MissingMainFunctionException::~MissingMainFunctionException()
{}

const char* MissingMainFunctionException::what() const throw()
{
	return message.c_str();
}