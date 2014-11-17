//
//  UnexpectedKeywordException.cpp
//  tokenizer
//
#include "UnexpectedKeywordException.h"

UnexpectedKeywordException::UnexpectedKeywordException(std::string m) : message(m)
{}

UnexpectedKeywordException::~UnexpectedKeywordException()
{}

const char* UnexpectedKeywordException::what() const throw()
{
	return message.c_str();
}