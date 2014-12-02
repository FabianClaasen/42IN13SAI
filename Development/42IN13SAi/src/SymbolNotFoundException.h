#pragma once

#include <exception>
#include <string>

class SymbolNotFoundException : public std::exception
{
private:
	std::string message;

public:
	const char* what() const throw();
	SymbolNotFoundException(std::string m);
	~SymbolNotFoundException();
};

