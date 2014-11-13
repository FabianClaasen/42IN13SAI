	//
	//  UnexpectedTypeException.h
	//  tokenizer
	//
#pragma once

#include <exception>
#include <string>

class UnexpectedTypeException : public std::exception
{
private:
	std::string message;

public:
	const char* what() const throw();
	UnexpectedTypeException(std::string m);
	~UnexpectedTypeException();
};