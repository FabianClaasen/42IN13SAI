//
//  ParseException.h
//  tokenizer
//
//  Created by Alex Nagelkerke on 20-09-14.
//  Copyright (c) 2014 Alex Nagelkerke. All rights reserved.
//

#ifndef PARNERNOTFOUNDEXCP_H_INCLUDED
#define PARNERNOTFOUNDEXCP_H_INCLUDED

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

#endif