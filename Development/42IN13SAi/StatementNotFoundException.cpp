//
//  StatementNotFoundException.cpp
//  tokenizer
//
#include "StatementNotFoundException.h"

StatementNotFoundException::StatementNotFoundException(std::string m) : message(m)
{}

StatementNotFoundException::~StatementNotFoundException()
{}

const char* StatementNotFoundException::what() const throw()
{
	return message.c_str();
}