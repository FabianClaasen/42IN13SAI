#pragma once

#include <exception>
#include <string>

class SaveException : public std::exception
{
private:
	std::string message;

public:
	const char* what() const throw();
	SaveException(std::string m);
	~SaveException();
};

