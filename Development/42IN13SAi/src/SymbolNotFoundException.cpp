#include "SymbolNotFoundException.h"

SymbolNotFoundException::SymbolNotFoundException(std::string m) : message(m)
{}

SymbolNotFoundException::~SymbolNotFoundException()
{}

const char* SymbolNotFoundException::what() const throw()
{
	return message.c_str();
}