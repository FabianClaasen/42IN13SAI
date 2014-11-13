//
//  ParameterNameException.h
//  tokenizer
//
#pragma once

#include <exception>
#include <string>

class ParameterNameException : public std::exception
{
private:
	std::string message;

public:
	const char* what() const throw();
	ParameterNameException(std::string m);
	~ParameterNameException();
};