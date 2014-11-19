#pragma once

#include <exception>
#include <string>

class MissingCompilerNodeException : public std::exception
{
private:
	std::string message;

public:
	const char* what() const throw();
	MissingCompilerNodeException(std::string m);
	~MissingCompilerNodeException();
};