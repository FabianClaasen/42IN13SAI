//
//  SubroutineNotFoundException.cpp
//  Virtual Machine
//

#include "SubroutineNotFoundException.h"

SubroutineNotFoundException::SubroutineNotFoundException(std::string m) : message(m)
{}

SubroutineNotFoundException::~SubroutineNotFoundException()
{}

const char* SubroutineNotFoundException::what() const throw()
{
	return message.c_str();
}