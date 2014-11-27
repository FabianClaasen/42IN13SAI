#include "ZeroDivideException.h"

ZeroDivideException::ZeroDivideException(std::string m) : message(m)
{}

ZeroDivideException::~ZeroDivideException()
{}

const char* ZeroDivideException::what() const throw()
{
	return message.c_str();
}