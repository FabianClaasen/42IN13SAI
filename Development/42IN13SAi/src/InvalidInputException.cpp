#include "InvalidInputException.h"

InvalidInputException::InvalidInputException(std::string m) : message(m)
{}

InvalidInputException::~InvalidInputException()
{}

const char* InvalidInputException::what() const throw()
{
	return message.c_str();
}