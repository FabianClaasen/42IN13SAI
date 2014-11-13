//
//  UnexpectedKeywordException.h
//  tokenizer
//
#pragma once

#include <exception>
#include <string>

class UnexpectedKeywordException : public std::exception
{
private:
	std::string message;

public:
	const char* what() const throw();
	UnexpectedKeywordException(std::string m);
	~UnexpectedKeywordException();
};