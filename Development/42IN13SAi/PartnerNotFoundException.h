//
//  ParseException.h
//  tokenizer
//
#pragma once

#include <exception>
#include <string>

class PartnerNotFoundException : public std::exception
{
private:
    std::string message;
    
public:
    const char* what() const throw();
    PartnerNotFoundException(std::string m);
};