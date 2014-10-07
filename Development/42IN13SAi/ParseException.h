//
//  ParseException.h
//  tokenizer
//
#pragma once

#include <exception>
#include <string>

class ParseException : public std::exception
{
private:
    std::string message;
    
public:
    const char* what() const throw();
    ParseException(std::string m);
};