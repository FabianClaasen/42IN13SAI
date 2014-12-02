	//
	//  VariableNotFoundException.h
	//  tokenizer
	//
#pragma once

#include <exception>
#include <string>

class VariableNotFoundException : public std::exception
{
private:
	std::string message;

public:
	const char* what() const throw();
	VariableNotFoundException(std::string m);
	~VariableNotFoundException();
};