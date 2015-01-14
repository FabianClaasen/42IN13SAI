#pragma once

#include <exception>
#include <string>

class LoadException : public std::exception
{
private:
	std::string message;

public:
	const char* what() const throw();
	LoadException(std::string m);
	~LoadException();
};

