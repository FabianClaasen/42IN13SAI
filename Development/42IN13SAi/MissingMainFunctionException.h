#pragma once

#include <exception>
#include <string>

class MissingMainFunctionException : public std::exception
{
private:
	std::string message;

public:
	const char* what() const throw();
	MissingMainFunctionException(std::string m);
	~MissingMainFunctionException();
};