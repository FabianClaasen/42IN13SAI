//
//  ParseException.cpp
//  tokenizer
//
//  Created by Alex Nagelkerke on 21-09-14.
//  Copyright (c) 2014 Alex Nagelkerke. All rights reserved.
//

#include <stdio.h>
#include "ParseException.h"

ParseException::ParseException(std::string m) : message(m)
{}


const char* ParseException::what() const throw()
{
    return message.c_str();
}