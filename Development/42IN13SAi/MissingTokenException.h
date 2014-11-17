//
//  MissingTokenException.h
//  tokenizer
//
#pragma once

#include <exception>
#include <string>

class MissingTokenException : public std::exception
{
private:
	std::string message;

public:
	const char* what() const throw();
	MissingTokenException(std::string m);
	~MissingTokenException();
};

