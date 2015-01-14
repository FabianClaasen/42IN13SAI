#include "LoadException.h"

LoadException::LoadException(std::string m) : message(m)
{}

LoadException::~LoadException()
{}

const char* LoadException::what() const throw()
{
	return message.c_str();
}