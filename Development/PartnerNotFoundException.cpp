//
//  ParseException.cpp
//  tokenizer
//
//  Created by Alex Nagelkerke on 20-09-14.
//  Copyright (c) 2014 Alex Nagelkerke. All rights reserved.
//

#include <stdio.h>
#include "PartnerNotFoundException.h"

PartnerNotFoundException::PartnerNotFoundException(std::string m) : message(m)
{}


const char* PartnerNotFoundException::what() const throw()
{
    return message.c_str();
}