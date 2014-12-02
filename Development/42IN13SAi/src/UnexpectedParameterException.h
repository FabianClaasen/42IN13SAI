//
//  UnexpectedParameterException.h
//  tokenizer
//
#pragma once

#include <exception>
#include <string>

class UnexpectedParameterException : public std::exception
{
private:
	std::string message;

public:
	const char* what() const throw();
	UnexpectedParameterException(std::string m);
	~UnexpectedParameterException();
};