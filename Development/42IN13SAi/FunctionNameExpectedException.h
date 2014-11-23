#pragma once

#include <exception>
#include <string>

class FunctionNameExpectedException : public std::exception
{
private:
	std::string message;

public:
	const char* what() const throw();
	FunctionNameExpectedException(std::string m);
	~FunctionNameExpectedException();
};