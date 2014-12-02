#pragma once

#include <exception>
#include <string>

class ZeroDivideException : public std::exception
{
private:
	std::string message;

public:
	const char* what() const throw();
	ZeroDivideException(std::string m);
	~ZeroDivideException();
};