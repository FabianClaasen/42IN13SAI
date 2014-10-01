//
//  ParseException.h
//  tokenizer
//
//  Created by Alex Nagelkerke on 21-09-14.
//  Copyright (c) 2014 Alex Nagelkerke. All rights reserved.
//

#ifndef PARSEEXCEPTION_H_INCLUDED
#define PARSEEXCEPTION_H_INCLUDED

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

#endif