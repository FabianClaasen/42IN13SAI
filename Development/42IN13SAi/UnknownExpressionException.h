#pragma once

#include <exception>
#include <string>

class UnknownExpressionException : public std::exception
{
private:
	std::string message;

public:
	const char* what() const throw();
	UnknownExpressionException(std::string m);
	~UnknownExpressionException();
};