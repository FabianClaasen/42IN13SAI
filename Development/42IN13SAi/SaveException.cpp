#include "SaveException.h"

SaveException::SaveException(std::string m) : message(m)
{}

SaveException::~SaveException()
{}

const char* SaveException::what() const throw()
{
	return message.c_str();
}