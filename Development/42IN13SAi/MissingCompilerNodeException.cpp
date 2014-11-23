//
//  MissingCompilerNodeException.cpp
//  Virtual Machine
//

#include "MissingCompilerNodeException.h"

MissingCompilerNodeException::MissingCompilerNodeException(std::string m) : message(m)
{}

MissingCompilerNodeException::~MissingCompilerNodeException()
{}

const char* MissingCompilerNodeException::what() const throw()
{
	return message.c_str();
}