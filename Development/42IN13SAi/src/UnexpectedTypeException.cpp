//
//  ParseException.cpp
//  tokenizer
//
#include "UnexpectedTypeException.h"

UnexpectedTypeException::UnexpectedTypeException(std::string m) : message(m)
{}

UnexpectedTypeException::~UnexpectedTypeException()
{}

const char* UnexpectedTypeException::what() const throw()
{
	return message.c_str();
}