	//
	//  StatementNotFoundException.h
	//  tokenizer
	//
#pragma once

#include <exception>
#include <string>

class StatementNotFoundException : public std::exception
{
private:
	std::string message;

public:
	const char* what() const throw();
	StatementNotFoundException(std::string m);
	~StatementNotFoundException();
};