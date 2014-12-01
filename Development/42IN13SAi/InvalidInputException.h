#pragma once

#include <exception>
#include <string>

class InvalidInputException : public std::exception
{
private:
	std::string message;

public:
	const char* what() const throw();
	InvalidInputException(std::string m);
	~InvalidInputException();
};