//
//  FunctionNameExpectedException.cpp
//  Virtual Machine
//

#include "FunctionNameExpectedException.h"

FunctionNameExpectedException::FunctionNameExpectedException(std::string m) : message(m)
{}

FunctionNameExpectedException::~FunctionNameExpectedException()
{}

const char* FunctionNameExpectedException::what() const throw()
{
	return message.c_str();
}