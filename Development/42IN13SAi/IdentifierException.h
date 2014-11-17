//
//  IdentifierException.h
//  tokenizer
//
#pragma once

#include <exception>
#include <string>

class IdentifierException : public std::exception
{
private:
	std::string message;

public:
	const char* what() const throw();
	IdentifierException(std::string m);
	~IdentifierException();
};