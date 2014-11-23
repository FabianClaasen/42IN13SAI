#pragma once

#include <exception>
#include <string>

class SubroutineNotFoundException : public std::exception
{
private:
	std::string message;

public:
	const char* what() const throw();
	SubroutineNotFoundException(std::string m);
	~SubroutineNotFoundException();
};